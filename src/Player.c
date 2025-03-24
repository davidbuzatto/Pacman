#include "Types.h"
#include "GameWorld.h"
#include "Player.h"
#include "raylib/raylib.h"

void inputAndUpdatePlayer( Player *player, float delta, int lines, int columns, int gridCellSize, GameWorld *gw ) {

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
        } else {
            //player->vel.x = 0;
        }

        if ( IsKeyDown( KEY_UP ) ) {
            player->vel.y = -player->walkingSpeed;
        } else if ( IsKeyDown( KEY_DOWN ) ) {
            player->vel.y = player->walkingSpeed;
        } else {
            //player->vel.y = 0;
        }

        player->pos.x += player->vel.x * delta;
        player->pos.y += player->vel.y * delta;

        if ( player->pos.x + player->radius < 0 ) {
            player->pos.x = columns * gridCellSize + gridCellSize / 2;
        } else if ( player->pos.x - player->radius > columns * gridCellSize ) {
            player->pos.x = -gridCellSize / 2;
        }

        if ( player->pos.y + player->radius < 0 ) {
            player->pos.y = lines * gridCellSize + gridCellSize / 2;
        } else if ( player->pos.y - player->radius > lines * gridCellSize ) {
            player->pos.y = -gridCellSize / 2;
        }

        Vector2 p = getPlayerLineAndColumn( player, gridCellSize );
        int x = (int) p.x;
        int y = (int) p.y;
        int xL = (int) ( ( player->pos.x - player->radius + 1 ) / gridCellSize );
        int xR = (int) ( ( player->pos.x + player->radius - 1 ) / gridCellSize );
        int yU = (int) ( ( player->pos.y - player->radius + 1 ) / gridCellSize );
        int yD = (int) ( ( player->pos.y + player->radius - 1 ) / gridCellSize );

        // collision
        if ( y >= 0 && y < lines ) {
            if ( xL >= 0 && xL < columns ) {
                if ( grid[y*columns+xL] < P ) {
                    player->pos.x = (xL+1) * gridCellSize + player->radius;
                    player->vel.x = 0;
                }
            }
            if ( xR >= 0 && xR < columns ) {
                if ( grid[y*columns+xR] < P ) {
                    player->pos.x = xR * gridCellSize - player->radius;
                    player->vel.x = 0;
                }
            }
        }

        if ( x >= 0 && x < columns ) {
            if ( yU >= 0 && yU < lines ) {
                if ( grid[yU*columns+x] < P ) {
                    player->pos.y = (yU+1) * gridCellSize + player->radius;
                    player->vel.y = 0;
                }
            }
            if ( yD >= 0 && yD < lines ) {
                if ( grid[yD*columns+x] < P ) {
                    player->pos.y = yD * gridCellSize - player->radius;
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
        if ( x >= 0 && x < columns && y >= 0 && y < lines ) {
            if ( grid[y*columns+x] == W ) {
                grid[y*columns+x] = P;
                player->points += 10;
            } else if ( grid[y*columns+x] == Q ) {
                grid[y*columns+x] = P;
                player->points += 50;
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

void drawPlayer( Player *player, int lines, int columns, int gridCellSize ) {

    float drawRadius = player->radius * 1.5;
    int x = columns * gridCellSize - gridCellSize - 10;
    int y = lines * gridCellSize + gridCellSize;
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

    DrawText( TextFormat( "%d", player->points ), 20, lines * gridCellSize + 5, 40, WHITE );

    for ( int i = 0; i < player->lives - 1; i++ ) {
        DrawCircleSector( (Vector2){ x - ( drawRadius * 2 + 10 ) * i, y }, drawRadius, -150, 150, 30, player->color );
    }

    /*Vector2 p = getPlayerLineAndColumn( player, gridCellSize );
    const char *t = TextFormat( "%d, %d", (int) p.x, (int) p.y );
    DrawText( t, player->pos.x, player->pos.y, 20, BLACK );*/

}

Vector2 getPlayerLineAndColumn( Player *player, int gridCellSize ) {
    return (Vector2) {
        player->pos.x / gridCellSize,
        player->pos.y / gridCellSize,
    };
}

Vector2 getLineAndColumnFromPos( Vector2 pos, int gridCellSize ) {
    return (Vector2) {
        pos.x / gridCellSize,
        pos.y / gridCellSize,
    };
}