#include <string.h>

#include "Types.h"
#include "Baddie.h"
#include "ResourceManager.h"
#include "utils.h"

#include "raylib/raylib.h"

const int SPRITE_MAP_START_X[] = { 2, 66, 130, 194 };

Baddie createNewBaddie( int line, int column, int ySource, Color color ) {

    return (Baddie) {

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
        .walkingSpeed = 200,
        .radius = GRID_CELL_SIZE / 2,
        .spriteMap = rm.spriteMap,
        .ySource = ySource,
        .color = color,

        .currentFrame = 0,
        .maxFrames = 2,
        .timeToNextFrame = 0.1,
        .frameTimeCounter = 0,

        .hunting = true,
        .timeToReturnToHunt = 5,
        .returnToHuntCounter = 0,

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

        .direction = DIRECTION_RIGHT,
        .state = ALIVE
        
    };

}

void updateBaddie( Baddie *baddie, float delta, GameWorld *gw ) {

    // counters and graphical state
    baddie->frameTimeCounter += delta;
    if ( baddie->frameTimeCounter > baddie->timeToNextFrame ) {
        baddie->frameTimeCounter = 0;
        baddie->currentFrame = ( baddie->currentFrame + 1 ) % baddie->maxFrames;
    }

    if ( !baddie->hunting ) {
        baddie->returnToHuntCounter += delta;
        if ( baddie->returnToHuntCounter > baddie->timeToReturnToHunt ) {
            baddie->hunting = true;
            baddie->returnToHuntCounter = 0;
            baddie->blinkCounter = 0;
            baddie->blink = false;
        }
        if ( baddie->returnToHuntCounter > baddie->timeToStartBlinking ) {
            baddie->blinkCounter += delta;
            if ( baddie->blinkCounter > baddie->timeToBlink ) {
                baddie->blinkCounter = 0;
                baddie->blink = !baddie->blink;
            }
        }
    }

    // path generation
    if ( baddie->currentPathPos == baddie->pathSize ) {
        baddie->currentPathPos = -1;
        baddie->vel.x = 0;
        baddie->vel.y = 0;
        generateNewRandomPath( baddie, gw );
        if ( baddie->state == RETURNING_HOME ) {
            baddie->hunting = true;
            baddie->blink = false;
            baddie->state = ALIVE;
        }
    }

    // direction and velocity calculation
    int destX = -1;
    int destY = -1;

    if ( baddie->currentPathPos != -1 ) {

        destX = baddie->path[baddie->currentPathPos].column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        destY = baddie->path[baddie->currentPathPos].line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;

        if ( destX < baddie->pos.x ) {
            baddie->vel.x = -baddie->walkingSpeed;
            baddie->direction = DIRECTION_LEFT;
        } else if ( destX > baddie->pos.x ) {
            baddie->vel.x = baddie->walkingSpeed;
            baddie->direction = DIRECTION_RIGHT;
        } else {
            baddie->vel.x = 0;
        }
        
        if ( destY < baddie->pos.y ) {
            baddie->vel.y = -baddie->walkingSpeed;
            baddie->direction = DIRECTION_UP;
        } else if ( destY > baddie->pos.y ) {
            baddie->vel.y = baddie->walkingSpeed;
            baddie->direction = DIRECTION_DOWN;
        } else {
            baddie->vel.y = 0;
        }

    }

    baddie->pos.x += baddie->vel.x * delta;
    baddie->pos.y += baddie->vel.y * delta;

    // path walking
    if ( destX != -1 || destY != -1 ) {
        switch ( baddie->direction ) {
            case DIRECTION_LEFT:
                if ( baddie->pos.x < destX ) {
                    baddie->pos.x = destX;
                    baddie->currentPathPos++;
                }
                break;
            case DIRECTION_RIGHT:
                if ( baddie->pos.x > destX ) {
                    baddie->pos.x = destX;
                    baddie->currentPathPos++;
                }
                break;
            case DIRECTION_UP:
                if ( baddie->pos.y < destY ) {
                    baddie->pos.y = destY;
                    baddie->currentPathPos++;
                }
                break;
            case DIRECTION_DOWN:
                if ( baddie->pos.y > destY ) {
                    baddie->pos.y = destY;
                    baddie->currentPathPos++;
                }
                break;
        }

    }

    // points from capturing
    if ( baddie->showCapturePoints ) {
        baddie->showPointsCounter += delta;
        if ( baddie->showPointsCounter > baddie->timeToShowPoints ) {
            baddie->showPointsCounter = 0;
            baddie->showCapturePoints = false;
        }
    }

}

void drawBaddie( Baddie *baddie ) {

    /*for ( int i = 0; i < baddie->pathSize - 1; i++ ) {
        CellPos *p1 = &baddie->path[i];
        CellPos *p2 = &baddie->path[i+1];
        DrawLine( 
            p1->column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p1->line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p2->column * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            p2->line * GRID_CELL_SIZE + GRID_CELL_SIZE / 2,
            baddie->color
        );
    }*/

    int xStart = 0;
    int yStart = baddie->ySource;

    if ( baddie->state == ALIVE ) {

        if ( baddie->hunting ) {
            switch ( baddie->direction ) {
                case DIRECTION_LEFT: xStart = SPRITE_MAP_START_X[1]; break;
                case DIRECTION_RIGHT: xStart = SPRITE_MAP_START_X[0]; break;
                case DIRECTION_UP: xStart = SPRITE_MAP_START_X[2]; break;
                case DIRECTION_DOWN: xStart = SPRITE_MAP_START_X[3]; break;
            }
        } else {
            if ( baddie->blink ) {
                xStart = 322;
                yStart = 130;
            } else {
                xStart = 258;
                yStart = 130;
            }
        }

        xStart += baddie->currentFrame * 32;
        
    } else if ( baddie->state == RETURNING_HOME ) {
        xStart = 258;
        yStart = 162;
        switch ( baddie->direction ) {
            case DIRECTION_LEFT: xStart += 32; break;
            case DIRECTION_RIGHT: xStart += 0; break;
            case DIRECTION_UP: xStart += 64; break;
            case DIRECTION_DOWN: xStart += 96; break;
        }
    }

    DrawTexturePro( 
        baddie->spriteMap, 
        (Rectangle){ xStart, yStart, 28, 28 }, 
        (Rectangle){ baddie->pos.x - 18, baddie->pos.y - 18, 36, 36 }, 
        (Vector2){ 0, 0 }, 
        0.0f, 
        WHITE
    );

    if ( baddie->showCapturePoints ) {
        const char *text = TextFormat( "%d", baddie->capturePoints );
        DrawText( 
            text, 
            baddie->capturePointsPos.x - MeasureText( text, 20 ) / 2,
            baddie->capturePointsPos.y - 20,
            20,
            RAYWHITE
        );
    }

}

void generateNewRandomPath( Baddie *baddie, GameWorld *gw ) {

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

    generateNewPath( baddie, targetLine, targetColumn, gw );

}

void generateNewPath( Baddie *baddie, int targetLine, int targetColumn, GameWorld *gw ) {

    Vector2 p = getLineAndColumn( baddie->pos );
    CellPos source = { p.y, p.x };
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
        baddie->path[pathSize-i-1] = current;
        current = gw->edgeTo[current.line][current.column];
        i++;
    }

    baddie->pathSize = pathSize;
    baddie->currentPathPos = 0;

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

void showCapturePoints( Baddie *baddie, Vector2 pos, int points ) {
    baddie->showCapturePoints = true;
    baddie->capturePointsPos.x = pos.x * GRID_CELL_SIZE;
    baddie->capturePointsPos.y = pos.y * GRID_CELL_SIZE;
    baddie->capturePoints = points;
}