#include "Types.h"
#include "Baddie.h"
#include "raylib/raylib.h"

const int xStarts[] = { 2, 66, 130, 194 };

void updateBaddie( Baddie *baddie, float delta, int lines, int columns, int gridCellSize, GameWorld *gw ) {

    CellType *grid = gw->grid;

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

    if ( baddie->currentPathPos == baddie->pathSize ) {
        baddie->currentPathPos = -1;
        baddie->vel.x = 0;
        baddie->vel.y = 0;
        generateNewPath( baddie, lines, columns, gridCellSize, gw );
    }

    Vector2 p = getBaddieLineAndColumnBaddie( baddie, gridCellSize );
    int destX = -1;
    int destY = -1;

    if ( baddie->currentPathPos != -1 ) {

        destX = baddie->path[baddie->currentPathPos].column * gridCellSize + gridCellSize / 2;
        destY = baddie->path[baddie->currentPathPos].line * gridCellSize + gridCellSize / 2;

        if ( destX < baddie->pos.x ) {
            baddie->vel.x = -baddie->walkingSpeed;
            baddie->direction = BADDIE_DIRECTION_LEFT;
        } else if ( destX > baddie->pos.x ) {
            baddie->vel.x = baddie->walkingSpeed;
            baddie->direction = BADDIE_DIRECTION_RIGHT;
        } else {
            baddie->vel.x = 0;
        }
        
        if ( destY < baddie->pos.y ) {
            baddie->vel.y = -baddie->walkingSpeed;
            baddie->direction = BADDIE_DIRECTION_UP;
        } else if ( destY > baddie->pos.y ) {
            baddie->vel.y = baddie->walkingSpeed;
            baddie->direction = BADDIE_DIRECTION_DOWN;
        } else {
            baddie->vel.y = 0;
        }

    }

    baddie->pos.x += baddie->vel.x * delta;
    baddie->pos.y += baddie->vel.y * delta;

    if ( destX != -1 || destY != -1 ) {
        switch ( baddie->direction ) {
            case BADDIE_DIRECTION_LEFT:
                if ( baddie->pos.x < destX ) {
                    baddie->pos.x = destX;
                    baddie->currentPathPos++;
                }
                break;
            case BADDIE_DIRECTION_RIGHT:
                if ( baddie->pos.x > destX ) {
                    baddie->pos.x = destX;
                    baddie->currentPathPos++;
                }
                break;
            case BADDIE_DIRECTION_UP:
                if ( baddie->pos.y < destY ) {
                    baddie->pos.y = destY;
                    baddie->currentPathPos++;
                }
                break;
            case BADDIE_DIRECTION_DOWN:
                if ( baddie->pos.y > destY ) {
                    baddie->pos.y = destY;
                    baddie->currentPathPos++;
                }
                break;
        }
    }

}

void drawBaddie( Baddie *baddie, int lines, int columns, int gridCellSize ) {

    /*float drawRadius = baddie->radius * 1.5;

    Color c = baddie->color;

    if ( !baddie->hunting ) {
        if ( baddie->blink ) {
            c = baddie->returningToHuntColor;
        } else {
            c = baddie->vulnerableColor;
        }
    }

    DrawCircleV( baddie->pos, drawRadius, c );*/

    /*for ( int i = 0; i < baddie->pathSize - 1; i++ ) {
        CellPos *p1 = &baddie->path[i];
        CellPos *p2 = &baddie->path[i+1];
        DrawLine( 
            p1->column * gridCellSize + gridCellSize / 2,
            p1->line * gridCellSize + gridCellSize / 2,
            p2->column * gridCellSize + gridCellSize / 2,
            p2->line * gridCellSize + gridCellSize / 2,
            baddie->color
        );
    }*/

    int xStart = 0;
    int yStart = baddie->ySource;

    if ( baddie->hunting ) {
        switch ( baddie->direction ) {
            case BADDIE_DIRECTION_LEFT: xStart = xStarts[1]; break;
            case BADDIE_DIRECTION_RIGHT: xStart = xStarts[0]; break;
            case BADDIE_DIRECTION_UP: xStart = xStarts[2]; break;
            case BADDIE_DIRECTION_DOWN: xStart = xStarts[3]; break;
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

    DrawTexturePro( 
        baddie->spriteMap, 
        (Rectangle){ xStart, yStart, 28, 28 }, 
        (Rectangle){ baddie->pos.x - 18, baddie->pos.y - 18, 36, 36 }, 
        (Vector2){ 0, 0 }, 
        0.0f, 
        WHITE
    );

}

Vector2 getBaddieLineAndColumnBaddie( Baddie *baddie, int gridCellSize ) {
    return (Vector2) {
        baddie->pos.x / gridCellSize,
        baddie->pos.y / gridCellSize,
    };
}

Vector2 getLineAndColumnFromPosBaddie( Vector2 pos, int gridCellSize ) {
    return (Vector2) {
        pos.x / gridCellSize,
        pos.y / gridCellSize,
    };
}

void generateNewPath( Baddie *baddie, int lines, int columns, int gridCellSize, GameWorld *gw ) {

    CellType *grid = gw->grid;
    
    bool ok = false;
    int line = 0;
    int column = 0;

    while ( !ok ) {
        line = GetRandomValue( 1, lines-1 );
        column = GetRandomValue( 1, columns-1 );
        //line = 11;
        //column = 14;
        //line = 1;
        //column = 6;
        if ( grid[line*columns+column] >= P ) {
            ok = true;
            //TraceLog( LOG_INFO, TextFormat( "ok: %d, %d", line, column ) );
        }
    }

    // BFS
    Vector2 p = getBaddieLineAndColumnBaddie( baddie, gridCellSize );
    bool marked[GRID_LINES*GRID_COLUMNS] = { 0 };
    CellPos edgeTo[GRID_LINES*GRID_COLUMNS] = { 0 };
    CellPos queue[200];
    int queueSize = 0;
    int queueStart = 0;
    int queueEnd = 0;
    CellPos source = { p.y, p.x };
    CellPos target = { line, column };
    queue[queueEnd++] = source;
    queueSize++;
    marked[source.line*columns+source.column] = true;

    //TraceLog( LOG_INFO, "from: %d, %d", source.line, source.column );
    //TraceLog( LOG_INFO, "to: %d, %d", target.line, target.column );

    while ( queueSize != 0 ) {

        CellPos v = queue[queueStart++];
        queueSize--;
        //TraceLog( LOG_INFO, TextFormat( "current: %d, %d", v.line, v.column ) );

        /*if ( v.line == target.line && v.column == target.column ) {
            break;
        }*/

        //TraceLog( LOG_INFO, "   ok" );
        
        int leftL = v.line;
        int leftC = v.column-1;

        if ( leftL >= 0 && leftL < lines &&
                leftC >= 0 && leftC < columns &&
                !marked[leftL*columns+leftC] &&
                grid[leftL*columns+leftC] >= P ) {
            edgeTo[leftL*columns+leftC] = v;
            marked[leftL*columns+leftC] = true;
            queue[queueEnd++] = (CellPos) { leftL, leftC };
            queueSize++;
        }

        int rightL = v.line;
        int rightC = v.column+1;

        if ( rightL >= 0 && rightL < lines &&
                rightC >= 0 && rightC < columns &&
                !marked[rightL*columns+rightC] &&
                grid[rightL*columns+rightC] >= P ) {
            edgeTo[rightL*columns+rightC] = v;
            marked[rightL*columns+rightC] = true;
            queue[queueEnd++] = (CellPos) { rightL, rightC };
            queueSize++;
        }

        int upL = v.line-1;
        int upC = v.column;

        if ( upL >= 0 && upL < lines &&
                upC >= 0 && upC < columns &&
                !marked[upL*columns+upC] &&
                grid[upL*columns+upC] >= P ) {
            edgeTo[upL*columns+upC] = v;
            marked[upL*columns+upC] = true;
            queue[queueEnd++] = (CellPos) { upL, upC };
            queueSize++;
        }

        int downL = v.line+1;
        int downC = v.column;

        if ( downL >= 0 && downL < lines &&
                downC >= 0 && downC < columns &&
                !marked[downL*columns+downC] &&
                grid[downL*columns+downC] >= P ) {
            edgeTo[downL*columns+downC] = v;
            marked[downL*columns+downC] = true;
            queue[queueEnd++] = (CellPos) { downL, downC };
            queueSize++;
        }

    }

    CellPos current = target;
    int pathSize = 0;
    while ( current.line != source.line || current.column != source.column ) {
        //TraceLog( LOG_INFO, TextFormat( "-> %d, %d", current.line, current.column ) );
        current = edgeTo[current.line*columns+current.column];
        pathSize++;
    }
    //TraceLog( LOG_INFO, TextFormat( "%d", pathSize ) );
    current = target;
    int i = 0;
    while ( current.line != source.line || current.column != source.column ) {        
        baddie->path[pathSize-i-1] = current;
        current = edgeTo[current.line*columns+current.column];
        i++;
    }
    baddie->pathSize = pathSize;
    baddie->currentPathPos = 0;

}