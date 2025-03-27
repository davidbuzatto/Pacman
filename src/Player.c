#include "Types.h"
#include "GameWorld.h"
#include "Player.h"
#include "utils.h"

#include "raylib/raylib.h"

const int PLAYER_LINE = 23;
const int PLAYER_COLUMN = 14;

Player createNewPlayer( void ) {

    return (Player) {

        .startPos = {
            .x = PLAYER_COLUMN * GRID_CELL_SIZE,
            .y = PLAYER_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
        },
        .pos = {
            .x = PLAYER_COLUMN * GRID_CELL_SIZE,
            .y = PLAYER_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
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

void inputAndUpdatePlayer( Player *player, float delta, GameWorld *gw ) {

    CellType *grid = gw->grid;

    if ( player->state == ALIVE ) {

        player->frameTimeCounter += delta;
        if ( player->frameTimeCounter > player->timeToNextFrame ) {
            player->frameTimeCounter = 0;
            player->currentFrame = ( player->currentFrame + 1 ) % player->maxFrames;
        }

        if ( IsKeyDown( KEY_LEFT ) ) {
            player->vel.x = -player->walkingSpeed;
        } else if ( IsKeyDown( KEY_RIGHT ) ) {
            player->vel.x = player->walkingSpeed;
        }

        if ( IsKeyDown( KEY_UP ) ) {
            player->vel.y = -player->walkingSpeed;
        } else if ( IsKeyDown( KEY_DOWN ) ) {
            player->vel.y = player->walkingSpeed;
        }

        player->pos.x += player->vel.x * delta;
        player->pos.y += player->vel.y * delta;

        // grid limits resolution
        if ( player->pos.x + player->radius < 0 ) {
            player->pos.x = GRID_COLUMNS * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        } else if ( player->pos.x - player->radius > GRID_COLUMNS * GRID_CELL_SIZE ) {
            player->pos.x = -GRID_CELL_SIZE / 2;
        }

        if ( player->pos.y + player->radius < 0 ) {
            player->pos.y = GRID_LINES * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
        } else if ( player->pos.y - player->radius > GRID_LINES * GRID_CELL_SIZE ) {
            player->pos.y = -GRID_CELL_SIZE / 2;
        }

        // obstacle collision resolution
        Vector2 p = getLineAndColumn( player->pos );
        int x = (int) p.x;
        int y = (int) p.y;
        int xL = (int) ( ( player->pos.x - player->radius + 1 ) / GRID_CELL_SIZE );
        int xR = (int) ( ( player->pos.x + player->radius - 1 ) / GRID_CELL_SIZE );
        int yU = (int) ( ( player->pos.y - player->radius + 1 ) / GRID_CELL_SIZE );
        int yD = (int) ( ( player->pos.y + player->radius - 1 ) / GRID_CELL_SIZE );

        if ( y >= 0 && y < GRID_LINES ) {
            if ( xL >= 0 && xL < GRID_COLUMNS ) {
                if ( grid[y*GRID_COLUMNS+xL] < P || grid[y*GRID_COLUMNS+xL] == M ) {
                    player->pos.x = (xL+1) * GRID_CELL_SIZE + player->radius;
                    player->vel.x = 0;
                }
            }
            if ( xR >= 0 && xR < GRID_COLUMNS ) {
                if ( grid[y*GRID_COLUMNS+xR] < P || grid[y*GRID_COLUMNS+xR] == M ) {
                    player->pos.x = xR * GRID_CELL_SIZE - player->radius;
                    player->vel.x = 0;
                }
            }
        }

        if ( x >= 0 && x < GRID_COLUMNS ) {
            if ( yU >= 0 && yU < GRID_LINES ) {
                if ( grid[yU*GRID_COLUMNS+x] < P || grid[yU*GRID_COLUMNS+x] == M ) {
                    player->pos.y = (yU+1) * GRID_CELL_SIZE + player->radius;
                    player->vel.y = 0;
                }
            }
            if ( yD >= 0 && yD < GRID_LINES ) {
                if ( grid[yD*GRID_COLUMNS+x] < P || grid[yD*GRID_COLUMNS+x] == M ) {
                    player->pos.y = yD * GRID_CELL_SIZE - player->radius;
                    player->vel.y = 0;
                }
            }
        }

        if ( player->vel.x < 0 ) {
            player->direction = DIRECTION_LEFT;
        } else if ( player->vel.x > 0 ) {
            player->direction = DIRECTION_RIGHT;
        }

        if ( player->vel.y < 0 ) {
            player->direction = DIRECTION_UP;
        } else if ( player->vel.y > 0 ) {
            player->direction = DIRECTION_DOWN;
        }

        // coins
        if ( x >= 0 && x < GRID_COLUMNS && y >= 0 && y < GRID_LINES ) {
            if ( grid[y*GRID_COLUMNS+x] == W ) {
                grid[y*GRID_COLUMNS+x] = P;
                player->points += 10;
                gw->remainingCoins--;
            } else if ( grid[y*GRID_COLUMNS+x] == Q ) {
                grid[y*GRID_COLUMNS+x] = P;
                player->points += 50;
                gw->remainingCoins--;
                startHuntingBaddies( gw );
            }
        }

    } else if ( player->state == DYING ) {

        if ( player->dyingCurrentFrame < player->dyingMaxFrames ) {
            player->dyingFrameTimeCounter += delta;
            if ( player->dyingFrameTimeCounter > player->dyingTimeToNextFrame ) {
                player->dyingFrameTimeCounter = 0;
                player->dyingCurrentFrame++;
            }
        }

    }

}

void drawPlayer( Player *player ) {

    float drawRadius = player->radius * 1.5;
    int x = GRID_COLUMNS * GRID_CELL_SIZE - GRID_CELL_SIZE - 10;
    int y = GRID_LINES * GRID_CELL_SIZE + GRID_CELL_SIZE;
    int m = 0;

    if ( player->state == ALIVE ) {

        switch ( player->direction ) {
            case DIRECTION_LEFT: m = 0; break;
            case DIRECTION_UP: m = 1; break;
            case DIRECTION_RIGHT: m = 2; break;
            case DIRECTION_DOWN: m = 3; break;
        }

        switch ( player->currentFrame ) {
            case 0:
                DrawCircleSector( player->pos, drawRadius, -150 + 90 * m, 150 + 90 * m, 30, player->color );
                break;
            case 1:
                DrawCircleSector( player->pos, drawRadius, -130 + 90 * m, 130 + 90 * m, 30, player->color );
                break;
            case 2:
                DrawCircleV( player->pos, drawRadius, player->color );
                break;
            default:
                DrawCircleV( player->pos, drawRadius, player->color );
                break;
        }

    } else if ( player->state == DYING ) {
        if ( player->dyingCurrentFrame == 0 ) {
            DrawCircleV( player->pos, drawRadius, player->color );
        } else if ( player->dyingCurrentFrame < player->dyingMaxFrames) {
            DrawCircleSector( player->pos, drawRadius, -85 + 15 * player->dyingCurrentFrame, 265 - 15 * player->dyingCurrentFrame, 30, player->color );
        } else {
            player->state = DEAD;
        }
    }

    DrawText( TextFormat( "%d", player->points ), 20, GRID_LINES * GRID_CELL_SIZE + 5, 40, WHITE );

    for ( int i = 0; i < player->lives - 1; i++ ) {
        DrawCircleSector( (Vector2){ x - ( drawRadius * 2 + 10 ) * i, y }, drawRadius, -150, 150, 30, player->color );
    }

}

Vector2 getPlayerLineAndColumn( Player *player ) {
    return (Vector2) {
        player->pos.x / GRID_CELL_SIZE,
        player->pos.y / GRID_CELL_SIZE,
    };
}