#include "Types.h"
#include "GameWorld.h"
#include "Pacman.h"
#include "utils.h"

#include "raylib/raylib.h"

Pacman createNewPacman( int line, int column ) {

    return (Pacman) {

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
        .walkingSpeed = 250,
        .radius = GRID_CELL_SIZE / 2,
        .color = { 255, 239, 0, 255 },

        .lives = 5,
        .points = 0,

        .currentFrame = 0,
        .maxFrames = 3,
        .timeToNextFrame = 0.05,
        .frameTimeCounter = 0,

        .dyingCurrentFrame = 0,
        .dyingMaxFrames = 11,
        .dyingTimeToNextFrame = 0.05,
        .dyingFrameTimeCounter = 0,

        .direction = DIRECTION_RIGHT,
        .state = ALIVE

    };

}

void inputAndUpdatePacman( Pacman *pacman, float delta, GameWorld *gw ) {

    CellType *grid = gw->grid;

    if ( pacman->state == ALIVE ) {

        pacman->frameTimeCounter += delta;
        if ( pacman->frameTimeCounter > pacman->timeToNextFrame ) {
            pacman->frameTimeCounter = 0;
            pacman->currentFrame = ( pacman->currentFrame + 1 ) % pacman->maxFrames;
        }

        if ( IsKeyDown( KEY_LEFT ) ) {
            pacman->vel.x = -pacman->walkingSpeed;
        } else if ( IsKeyDown( KEY_RIGHT ) ) {
            pacman->vel.x = pacman->walkingSpeed;
        }

        if ( IsKeyDown( KEY_UP ) ) {
            pacman->vel.y = -pacman->walkingSpeed;
        } else if ( IsKeyDown( KEY_DOWN ) ) {
            pacman->vel.y = pacman->walkingSpeed;
        }

        pacman->pos.x += pacman->vel.x * delta;
        pacman->pos.y += pacman->vel.y * delta;

        // grid limits resolution
        if ( pacman->pos.x + pacman->radius < 0 ) {
            pacman->pos.x = GRID_COLUMNS * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        } else if ( pacman->pos.x - pacman->radius > GRID_COLUMNS * GRID_CELL_SIZE ) {
            pacman->pos.x = -GRID_CELL_SIZE / 2;
        }

        if ( pacman->pos.y + pacman->radius < 0 ) {
            pacman->pos.y = GRID_LINES * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        } else if ( pacman->pos.y - pacman->radius > GRID_LINES * GRID_CELL_SIZE ) {
            pacman->pos.y = -GRID_CELL_SIZE / 2;
        }

        // obstacle collision resolution
        Vector2 p = getLineAndColumn( pacman->pos );
        int x = (int) p.x;
        int y = (int) p.y;
        int xL = (int) ( ( pacman->pos.x - pacman->radius + 1 ) / GRID_CELL_SIZE );
        int xR = (int) ( ( pacman->pos.x + pacman->radius - 1 ) / GRID_CELL_SIZE );
        int yU = (int) ( ( pacman->pos.y - pacman->radius + 1 ) / GRID_CELL_SIZE );
        int yD = (int) ( ( pacman->pos.y + pacman->radius - 1 ) / GRID_CELL_SIZE );

        if ( y >= 0 && y < GRID_LINES ) {
            if ( xL >= 0 && xL < GRID_COLUMNS ) {
                if ( grid[y*GRID_COLUMNS+xL] < P || grid[y*GRID_COLUMNS+xL] == M ) {
                    pacman->pos.x = (xL+1) * GRID_CELL_SIZE + pacman->radius;
                    pacman->vel.x = 0;
                }
            }
            if ( xR >= 0 && xR < GRID_COLUMNS ) {
                if ( grid[y*GRID_COLUMNS+xR] < P || grid[y*GRID_COLUMNS+xR] == M ) {
                    pacman->pos.x = xR * GRID_CELL_SIZE - pacman->radius;
                    pacman->vel.x = 0;
                }
            }
        }

        if ( x >= 0 && x < GRID_COLUMNS ) {
            if ( yU >= 0 && yU < GRID_LINES ) {
                if ( grid[yU*GRID_COLUMNS+x] < P || grid[yU*GRID_COLUMNS+x] == M ) {
                    pacman->pos.y = (yU+1) * GRID_CELL_SIZE + pacman->radius;
                    pacman->vel.y = 0;
                }
            }
            if ( yD >= 0 && yD < GRID_LINES ) {
                if ( grid[yD*GRID_COLUMNS+x] < P || grid[yD*GRID_COLUMNS+x] == M ) {
                    pacman->pos.y = yD * GRID_CELL_SIZE - pacman->radius;
                    pacman->vel.y = 0;
                }
            }
        }

        if ( pacman->vel.x < 0 ) {
            pacman->direction = DIRECTION_LEFT;
        } else if ( pacman->vel.x > 0 ) {
            pacman->direction = DIRECTION_RIGHT;
        }

        if ( pacman->vel.y < 0 ) {
            pacman->direction = DIRECTION_UP;
        } else if ( pacman->vel.y > 0 ) {
            pacman->direction = DIRECTION_DOWN;
        }

        // coins
        if ( x >= 0 && x < GRID_COLUMNS && y >= 0 && y < GRID_LINES ) {
            if ( grid[y*GRID_COLUMNS+x] == W ) {
                grid[y*GRID_COLUMNS+x] = P;
                pacman->points += 10;
                gw->remainingCoins--;
            } else if ( grid[y*GRID_COLUMNS+x] == Q ) {
                grid[y*GRID_COLUMNS+x] = P;
                pacman->points += 50;
                gw->remainingCoins--;
                startScatterGhosts( gw );
            }
        }

    } else if ( pacman->state == DYING ) {

        if ( pacman->dyingCurrentFrame < pacman->dyingMaxFrames ) {
            pacman->dyingFrameTimeCounter += delta;
            if ( pacman->dyingFrameTimeCounter > pacman->dyingTimeToNextFrame ) {
                pacman->dyingFrameTimeCounter = 0;
                pacman->dyingCurrentFrame++;
            }
        }

    }

}

void drawPacman( Pacman *pacman ) {

    float drawRadius = pacman->radius * 1.5;
    int x = GRID_COLUMNS * GRID_CELL_SIZE - GRID_CELL_SIZE - 10;
    int y = GRID_LINES * GRID_CELL_SIZE + GRID_CELL_SIZE;
    int m = 0;

    if ( pacman->state == ALIVE ) {

        switch ( pacman->direction ) {
            case DIRECTION_LEFT: m = 0; break;
            case DIRECTION_UP: m = 1; break;
            case DIRECTION_RIGHT: m = 2; break;
            case DIRECTION_DOWN: m = 3; break;
        }

        switch ( pacman->currentFrame ) {
            case 0:
                DrawCircleSector( pacman->pos, drawRadius, -150 + 90 * m, 150 + 90 * m, 30, pacman->color );
                break;
            case 1:
                DrawCircleSector( pacman->pos, drawRadius, -130 + 90 * m, 130 + 90 * m, 30, pacman->color );
                break;
            case 2:
                DrawCircleV( pacman->pos, drawRadius, pacman->color );
                break;
            default:
                DrawCircleV( pacman->pos, drawRadius, pacman->color );
                break;
        }

    } else if ( pacman->state == DYING ) {
        if ( pacman->dyingCurrentFrame == 0 ) {
            DrawCircleV( pacman->pos, drawRadius, pacman->color );
        } else if ( pacman->dyingCurrentFrame < pacman->dyingMaxFrames) {
            DrawCircleSector( pacman->pos, drawRadius, -85 + 15 * pacman->dyingCurrentFrame, 265 - 15 * pacman->dyingCurrentFrame, 30, pacman->color );
        } else {
            pacman->state = DEAD;
        }
    }

    DrawText( TextFormat( "%d", pacman->points ), 20, GRID_LINES * GRID_CELL_SIZE + 5, 40, WHITE );

    for ( int i = 0; i < pacman->lives - 1; i++ ) {
        DrawCircleSector( (Vector2){ x - ( drawRadius * 2 + 10 ) * i, y }, drawRadius, -150, 150, 30, pacman->color );
    }

}