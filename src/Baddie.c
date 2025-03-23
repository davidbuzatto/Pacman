#include "Types.h"
#include "Baddie.h"
#include "raylib/raylib.h"

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

    baddie->pos.x += baddie->vel.x * delta;
    baddie->pos.y += baddie->vel.y * delta;

    if ( baddie->pos.x + baddie->radius < 0 ) {
        baddie->pos.x = columns * gridCellSize + gridCellSize / 2;
    } else if ( baddie->pos.x - baddie->radius > columns * gridCellSize ) {
        baddie->pos.x = -gridCellSize / 2;
    }

    if ( baddie->pos.y + baddie->radius < 0 ) {
        baddie->pos.y = lines * gridCellSize + gridCellSize / 2;
    } else if ( baddie->pos.y - baddie->radius > lines * gridCellSize ) {
        baddie->pos.y = -gridCellSize / 2;
    }

    Vector2 p = getBaddieLineAndColumnBaddie( baddie, gridCellSize );
    int x = (int) p.x;
    int y = (int) p.y;
    int xL = (int) ( ( baddie->pos.x - baddie->radius + 1 ) / gridCellSize );
    int xR = (int) ( ( baddie->pos.x + baddie->radius - 1 ) / gridCellSize );
    int yU = (int) ( ( baddie->pos.y - baddie->radius + 1 ) / gridCellSize );
    int yD = (int) ( ( baddie->pos.y + baddie->radius - 1 ) / gridCellSize );

    // collision
    if ( y >= 0 && y < lines ) {
        if ( xL >= 0 && xL < columns ) {
            if ( grid[y*columns+xL] < P ) {
                baddie->pos.x = (xL+1) * gridCellSize + baddie->radius;
                baddie->vel.x = 0;
            }
        }
        if ( xR >= 0 && xR < columns ) {
            if ( grid[y*columns+xR] < P ) {
                baddie->pos.x = xR * gridCellSize - baddie->radius;
                baddie->vel.x = 0;
            }
        }
    }

    if ( x >= 0 && x < columns ) {
        if ( yU >= 0 && yU < lines ) {
            if ( grid[yU*columns+x] < P ) {
                baddie->pos.y = (yU+1) * gridCellSize + baddie->radius;
                baddie->vel.y = 0;
            }
        }
        if ( yD >= 0 && yD < lines ) {
            if ( grid[yD*columns+x] < P ) {
                baddie->pos.y = yD * gridCellSize - baddie->radius;
                baddie->vel.y = 0;
            }
        }
    }

    if ( baddie->vel.x < 0 ) {
        baddie->direction = BADDIE_DIRECTION_LEFT;
    } else if ( baddie->vel.x > 0 ) {
        baddie->direction = BADDIE_DIRECTION_RIGHT;
    }

    if ( baddie->vel.y < 0 ) {
        baddie->direction = BADDIE_DIRECTION_UP;
    } else if ( baddie->vel.y > 0 ) {
        baddie->direction = BADDIE_DIRECTION_DOWN;
    }

}

void drawBaddie( Baddie *baddie, int lines, int columns, int gridCellSize ) {

    float drawRadius = baddie->radius * 1.5;

    Color c = baddie->color;

    if ( !baddie->hunting ) {
        if ( baddie->blink ) {
            c = baddie->returningToHuntColor;
        } else {
            c = baddie->vulnerableColor;
        }
    }

    DrawCircleV( baddie->pos, drawRadius, c );
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