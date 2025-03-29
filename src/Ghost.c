#include <string.h>
#include <math.h>

#include "Types.h"
#include "Ghost.h"
#include "ResourceManager.h"
#include "utils.h"

#include "raylib/raylib.h"

Ghost createNewGhost( int line, int column, int ySource, const char *name, ChaseTargetType chaseTarget, Quadrant scatterQuadrant, Color color ) {

    return (Ghost) {

        .startPos = {
            .x = column * GRID_CELL_SIZE,
            .y = line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
        },
        .pos = {
            .x = column * GRID_CELL_SIZE,
            .y = line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
        },
        .vel = {
            .x = 0,
            .y = 0
        },
        .chasingSpeed = 200,
        .scatterSpeed = 150,
        .returningHomeSpeed = 300,
        .radius = GRID_CELL_SIZE / 2,
        .spriteMap = rm.spriteMap,
        .ySource = ySource,
        .name = name,
        .color = color,

        .currentFrame = 0,
        .maxFrames = 2,
        .timeToNextFrame = 0.1,
        .frameTimeCounter = 0,

        .chasing = true,
        .timeToReturnToChase = 5,
        .returnToChaseCounter = 0,

        .blink = false,
        .timeToStartBlinking = 3,
        .timeToBlink = 0.2,
        .blinkCounter = 0,

        .path = { 0 },
        .pathSize = 0,
        .currentPathPos = 1,

        .showCapturePoints = false,
        .capturePointsPos = { 0 },
        .capturePoints = 0,
        .timeToShowPoints = 2,
        .showPointsCounter = 0,

        .chaseTarget = chaseTarget,
        .scatterQuadrant = scatterQuadrant,

        .direction = DIRECTION_RIGHT,
        .state = ALIVE
        
    };

}

void updateGhost( Ghost *ghost, float delta, GameWorld *gw ) {

    // counters and graphical state
    ghost->frameTimeCounter += delta;
    if ( ghost->frameTimeCounter > ghost->timeToNextFrame ) {
        ghost->frameTimeCounter = 0;
        ghost->currentFrame = ( ghost->currentFrame + 1 ) % ghost->maxFrames;
    }

    if ( !ghost->chasing ) {
        ghost->returnToChaseCounter += delta;
        if ( ghost->returnToChaseCounter > ghost->timeToReturnToChase ) {
            ghost->chasing = true;
            ghost->returnToChaseCounter = 0;
            ghost->blinkCounter = 0;
            ghost->blink = false;
        }
        if ( ghost->returnToChaseCounter > ghost->timeToStartBlinking ) {
            ghost->blinkCounter += delta;
            if ( ghost->blinkCounter > ghost->timeToBlink ) {
                ghost->blinkCounter = 0;
                ghost->blink = !ghost->blink;
            }
        }
    }

    // path generation
    if ( ghost->currentPathPos == ghost->pathSize ) {
        ghost->currentPathPos = -1;
        ghost->vel.x = 0;
        ghost->vel.y = 0;
        if ( ghost->chasing ) {
            generateChaseTargetPath( ghost, gw );
        } else {
            generateScatterQuadrantPath( ghost, gw );
        }
        if ( ghost->state == RETURNING_HOME ) {
            ghost->chasing = true;
            ghost->blink = false;
            ghost->state = ALIVE;
        }
    }

    // direction and velocity calculation
    int destX = -1;
    int destY = -1;

    if ( ghost->currentPathPos != -1 ) {

        destX = ghost->path[ghost->currentPathPos].column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        destY = ghost->path[ghost->currentPathPos].line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;

        float currentSpeed = ghost->chasingSpeed;

        if ( ghost->state == RETURNING_HOME ) {
            currentSpeed = ghost->returningHomeSpeed;
        } else if ( !ghost->chasing ) {
            currentSpeed = ghost->scatterSpeed;
        }

        if ( destX < ghost->pos.x ) {
            ghost->vel.x = -currentSpeed;
            ghost->direction = DIRECTION_LEFT;
        } else if ( destX > ghost->pos.x ) {
            ghost->vel.x = currentSpeed;
            ghost->direction = DIRECTION_RIGHT;
        } else {
            ghost->vel.x = 0;
        }
        
        if ( destY < ghost->pos.y ) {
            ghost->vel.y = -currentSpeed;
            ghost->direction = DIRECTION_UP;
        } else if ( destY > ghost->pos.y ) {
            ghost->vel.y = currentSpeed;
            ghost->direction = DIRECTION_DOWN;
        } else {
            ghost->vel.y = 0;
        }

    }

    ghost->pos.x += ghost->vel.x * delta;
    ghost->pos.y += ghost->vel.y * delta;

    // path walking
    if ( destX != -1 || destY != -1 ) {
        switch ( ghost->direction ) {
            case DIRECTION_LEFT:
                if ( ghost->pos.x < destX ) {
                    ghost->pos.x = destX;
                    ghost->currentPathPos++;
                }
                break;
            case DIRECTION_RIGHT:
                if ( ghost->pos.x > destX ) {
                    ghost->pos.x = destX;
                    ghost->currentPathPos++;
                }
                break;
            case DIRECTION_UP:
                if ( ghost->pos.y < destY ) {
                    ghost->pos.y = destY;
                    ghost->currentPathPos++;
                }
                break;
            case DIRECTION_DOWN:
                if ( ghost->pos.y > destY ) {
                    ghost->pos.y = destY;
                    ghost->currentPathPos++;
                }
                break;
        }

    }

    // points from capturing
    if ( ghost->showCapturePoints ) {
        ghost->showPointsCounter += delta;
        if ( ghost->showPointsCounter > ghost->timeToShowPoints ) {
            ghost->showPointsCounter = 0;
            ghost->showCapturePoints = false;
        }
    }

}

void drawGhost( Ghost *ghost ) {

    for ( int i = 0; i < ghost->pathSize - 1; i++ ) {
        CellPos *p1 = &ghost->path[i];
        CellPos *p2 = &ghost->path[i+1];
        DrawLine( 
            p1->column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p1->line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p2->column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p2->line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            ghost->color
        );
    }

    int xStart = 2;
    int yStart = ghost->ySource;

    if ( ghost->state == ALIVE ) {

        if ( ghost->chasing ) {
            switch ( ghost->direction ) {
                // sprite map offsets
                case DIRECTION_LEFT: xStart += 64; break;
                case DIRECTION_RIGHT: xStart += 0; break;
                case DIRECTION_UP: xStart += 128; break;
                case DIRECTION_DOWN: xStart += 192; break;
            }
        } else {
            // scatter mode sprite position in sprite map
            if ( ghost->blink ) {
                xStart = 322;
                yStart = 130;
            } else {
                xStart = 258;
                yStart = 130;
            }
        }

        xStart += ghost->currentFrame * 32;
        
    } else if ( ghost->state == RETURNING_HOME ) {
        // eyes position in sprite map
        xStart = 258;
        yStart = 162;
        switch ( ghost->direction ) {
            // sprite map offsets
            case DIRECTION_LEFT: xStart += 32; break;
            case DIRECTION_RIGHT: xStart += 0; break;
            case DIRECTION_UP: xStart += 64; break;
            case DIRECTION_DOWN: xStart += 96; break;
        }
    }

    DrawTexturePro( 
        ghost->spriteMap, 
        (Rectangle){ xStart, yStart, 28, 28 }, 
        (Rectangle){ ghost->pos.x - 18, ghost->pos.y - 18, 36, 36 }, 
        (Vector2){ 0, 0 }, 
        0.0f, 
        WHITE
    );

    if ( ghost->showCapturePoints ) {
        const char *text = TextFormat( "%d", ghost->capturePoints );
        DrawText( 
            text, 
            ghost->capturePointsPos.x - MeasureText( text, 20 ) / 2,
            ghost->capturePointsPos.y - 20,
            20,
            RAYWHITE
        );
    }

    /*DrawText( ghost->name, ghost->pos.x + 30, ghost->pos.y, 20, ghost->color );
    DrawRectangle( 
        ghost->scatterQuadrant.column1 * GRID_CELL_SIZE,
        ghost->scatterQuadrant.line1 * GRID_CELL_SIZE,
        ( ghost->scatterQuadrant.column2 - ghost->scatterQuadrant.column1 + 1 ) * GRID_CELL_SIZE,
        ( ghost->scatterQuadrant.line2 - ghost->scatterQuadrant.line1 + 1 ) * GRID_CELL_SIZE,
        Fade( ghost->color, 0.5f )
    );*/
    //DrawCircleLinesV( ghost->pos, GRID_CELL_SIZE * 8, ghost->color );

}

void generateNewRandomPath( Ghost *ghost, GameWorld *gw ) {

    CellType *grid = gw->grid;

    bool ok = false;
    int targetLine = 0;
    int targetColumn = 0;

    while ( !ok ) {
        targetLine = GetRandomValue( 1, GRID_LINES-1 );
        targetColumn = GetRandomValue( 1, GRID_COLUMNS-1 );
        if ( grid[targetLine*GRID_COLUMNS+targetColumn] >= P ) {
            ok = true;
        }
    }

    generateNewPath( ghost, targetLine, targetColumn, gw );

}

void generateScatterQuadrantPath( Ghost *ghost, GameWorld *gw ) {

    CellType *grid = gw->grid;

    bool ok = false;
    int targetLine = 0;
    int targetColumn = 0;

    while ( !ok ) {
        targetLine = GetRandomValue( ghost->scatterQuadrant.line1, ghost->scatterQuadrant.line2 );
        targetColumn = GetRandomValue( ghost->scatterQuadrant.column1, ghost->scatterQuadrant.column2 );
        if ( grid[targetLine*GRID_COLUMNS+targetColumn] >= P ) {
            ok = true;
        }
    }

    generateNewPath( ghost, targetLine, targetColumn, gw );

}

void generateChaseTargetPath( Ghost *ghost, GameWorld *gw ) {

    switch ( ghost->chaseTarget ) {
        case CHASE_TARGET_TYPE_BLINKY:
            generateBlinkyChasePath( ghost, gw );
            break;
        case CHASE_TARGET_TYPE_INKY:
            generateInkyChasePath( ghost, gw );
            break;
        case CHASE_TARGET_TYPE_PINKY:
            generatePinkyChasePath( ghost, gw );
            break;
        case CHASE_TARGET_TYPE_CLYDE:
            generateClydeChasePath( ghost, gw );
            break;
    }

}

// follows Pac-man
void generateBlinkyChasePath( Ghost *ghost, GameWorld *gw ) {

    CellPos pos = getLineAndColumn( gw->pacman.pos );
    generateNewPath( ghost, pos.line, pos.column, gw );

}

// follows Pac-man (distance of 2 cells)
void generateInkyChasePath( Ghost *ghost, GameWorld *gw ) {

    CellType *grid = gw->grid;
    CellPos pos = getLineAndColumn( gw->pacman.pos );

    int targetLine = 0;
    int targetColumn = 0;

    int incPos[] = { 0, 2, 0, -2, 2, 0, -2, 0 };
    int k = 0;

    while ( true ) {
        targetLine = pos.line + incPos[k];
        targetColumn = pos.column + incPos[k+1];
        if ( grid[targetLine*GRID_COLUMNS+targetColumn] >= P ) {
            generateNewPath( ghost, targetLine, targetColumn, gw );
            break;
        }
        k += 2;
        if ( k > 7 ) {
            generateNewRandomPath( ghost, gw );
            break;
        }
    }

}

// follows Pac-man (distance of 2 cells)
void generatePinkyChasePath( Ghost *ghost, GameWorld *gw ) {
    generateInkyChasePath( ghost, gw );
}

// follows Pac-man and evades when next to 8 cells
void generateClydeChasePath( Ghost *ghost, GameWorld *gw ) {

    float dist = hypotf( ghost->pos.x - gw->pacman.pos.x, ghost->pos.y - gw->pacman.pos.y );

    if ( dist > GRID_CELL_SIZE * 8 ) {
        CellPos pos = getLineAndColumn( gw->pacman.pos );
        generateNewPath( ghost, pos.line, pos.column, gw );
    } else {
        generateScatterQuadrantPath( ghost, gw );
    }

}

void generateNewPath( Ghost *ghost, int targetLine, int targetColumn, GameWorld *gw ) {

    CellPos source = getLineAndColumn( ghost->pos );
    CellPos target = { targetLine, targetColumn };

    int queueSize = 0;
    int queueStart = 0;
    int queueEnd = 0;

    // BFS
    memset( gw->marked, 0, sizeof( bool ) * GRID_LINES * GRID_COLUMNS );
    gw->marked[source.line][source.column] = true;

    gw->queue[queueEnd++] = source;
    queueSize++;

    while ( queueSize != 0 ) {

        CellPos cell = gw->queue[queueStart++];
        queueSize--;

        if ( cell.line == target.line && cell.column == target.column ) {
            break;
        }
        
        visitNeightbor( cell.line, cell.column-1, cell, &queueSize, &queueEnd, gw );
        visitNeightbor( cell.line, cell.column+1, cell, &queueSize, &queueEnd, gw );
        visitNeightbor( cell.line-1, cell.column, cell, &queueSize, &queueEnd, gw );
        visitNeightbor( cell.line+1, cell.column, cell, &queueSize, &queueEnd, gw );

    }

    // builds the new path from source to target
    CellPos current = target;
    int pathSize = 0;
    while ( current.line != source.line || current.column != source.column ) {
        current = gw->edgeTo[current.line][current.column];
        pathSize++;
    }

    current = target;
    int i = 0;
    while ( current.line != source.line || current.column != source.column ) {        
        ghost->path[pathSize-i-1] = current;
        current = gw->edgeTo[current.line][current.column];
        i++;
    }

    ghost->pathSize = pathSize;
    ghost->currentPathPos = 0;

}

void visitNeightbor( int line, int column, CellPos cell, int *queueSize, int *queueEnd, GameWorld *gw ) {

    if ( line >= 0 && line < GRID_LINES &&
         column >= 0 && column < GRID_COLUMNS &&
         !gw->marked[line][column] &&
         gw->grid[line*GRID_COLUMNS+column] >= G ) {

        gw->edgeTo[line][column] = cell;
        gw->marked[line][column] = true;
        gw->queue[(*queueEnd)++] = (CellPos) { line, column };
        (*queueSize)++;

    }

}

void showCapturePoints( Ghost *ghost, CellPos pos, int points ) {
    ghost->showCapturePoints = true;
    ghost->capturePointsPos.x = pos.column * GRID_CELL_SIZE;
    ghost->capturePointsPos.y = pos.line * GRID_CELL_SIZE;
    ghost->capturePoints = points;
}