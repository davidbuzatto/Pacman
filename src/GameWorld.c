/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Types.h"
#include "GameWorld.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

const int SW = GRID_CELL_SIZE / 3;

const int PLAYER_LINE = 23;
const int PLAYER_COLUMN = 14;

const int BADDIE_LINE = 14;
const int BADDIE_COLUMN = 14;

const float TIME_TO_RETURN_TO_HUNT = 5;
const float TIME_TO_START_BLINKING = 3;
const float TIME_TO_BLINK = 0.2;

const bool DEBUG_GRID = false;
const bool IMMORTAL = false;
const int BADDIES_TO_RUN = 4;

const CellType templateGrid[] = {
    A, H, H, H, H, H, H, H, H, H, H, H, H, B,   A, H, H, H, H, H, H, H, H, H, H, H, H, B,
    V, W, W, W, W, W, W, W, W, W, W, W, W, V,   V, W, W, W, W, W, W, W, W, W, W, W, W, V,
    V, W, A, H, H, B, W, A, H, H, H, B, W, V,   V, W, A, H, H, H, B, W, A, H, H, B, W, V,
    V, Q, V, O, O, V, W, V, O, O, O, V, W, V,   V, W, V, O, O, O, V, W, V, O, O, V, Q, V,
    V, W, C, H, H, D, W, C, H, H, H, D, W, C,   D, W, C, H, H, H, D, W, C, H, H, D, W, V,
    V, W, W, W, W, W, W, W, W, W, W, W, W, W,   W, W, W, W, W, W, W, W, W, W, W, W, W, V,
    V, W, A, H, H, B, W, A, B, W, A, H, H, H,   H, H, H, B, W, A, B, W, A, H, H, B, W, V,
    V, W, C, H, H, D, W, V, V, W, C, H, H, B,   A, H, H, D, W, V, V, W, C, H, H, D, W, V,
    V, W, W, W, W, W, W, V, V, W, W, W, W, V,   V, W, W, W, W, V, V, W, W, W, W, W, W, V,
    C, H, H, H, H, B, W, V, C, H, H, B, P, V,   V, P, A, H, H, D, V, W, A, H, H, H, H, D,
    O, O, O, O, O, V, W, V, A, H, H, D, P, C,   D, P, C, H, H, B, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, P, P, P, P,   P, P, P, P, P, V, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, A, H, H, M,   M, H, H, B, P, V, V, W, V, O, O, O, O, O,
    H, H, H, H, H, D, W, C, D, P, V, P, P, P,   P, P, P, V, P, C, D, W, C, H, H, H, H, H,
    P, P, P, P, P, P, W, P, P, P, V, P, P, P,   P, P, P, V, P, P, P, W, P, P, P, P, P, P,
    H, H, H, H, H, B, W, A, B, P, V, P, P, P,   P, P, P, V, P, A, B, W, A, H, H, H, H, H,
    O, O, O, O, O, V, W, V, V, P, C, H, H, H,   H, H, H, D, P, V, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, P, P, P, P,   P, P, P, P, P, V, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, A, H, H, H,   H, H, H, B, P, V, V, W, V, O, O, O, O, O,
    A, H, H, H, H, D, W, C, D, P, C, H, H, B,   A, H, H, D, P, C, D, W, C, H, H, H, H, B,
    V, W, W, W, W, W, W, W, W, W, W, W, W, V,   V, W, W, W, W, W, W, W, W, W, W, W, W, V,
    V, W, A, H, H, B, W, A, H, H, H, B, W, V,   V, W, A, H, H, H, B, W, A, H, H, B, W, V,
    V, W, C, H, B, V, W, C, H, H, H, D, W, C,   D, W, C, H, H, H, D, W, V, A, H, D, W, V,
    V, Q, W, W, V, V, W, W, W, W, W, W, W, P,   P, W, W, W, W, W, W, W, V, V, W, W, Q, V,
    C, H, B, W, V, V, W, A, B, W, A, H, H, H,   H, H, H, B, W, A, B, W, V, V, W, A, H, D,
    A, H, D, W, C, D, W, V, V, W, C, H, H, B,   A, H, H, D, W, V, V, W, C, D, W, C, H, B,
    V, W, W, W, W, W, W, V, V, W, W, W, W, V,   V, W, W, W, W, V, V, W, W, W, W, W, W, V,
    V, W, A, H, H, H, H, D, C, H, H, B, W, V,   V, W, A, H, H, D, C, H, H, H, H, B, W, V,
    V, W, C, H, H, H, H, H, H, H, H, D, W, C,   D, W, C, H, H, H, H, H, H, H, H, D, W, V,
    V, W, W, W, W, W, W, W, W, W, W, W, W, W,   W, W, W, W, W, W, W, W, W, W, W, W, W, V,
    C, H, H, H, H, H, H, H, H, H, H, H, H, H,   H, H, H, H, H, H, H, H, H, H, H, H, H, D
};

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    
    *gw = (GameWorld) {
        .grid = {0},
        .player = {
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
            .direction = DIRECTION_RIGHT,
            .state = ALIVE,
            .dyingCurrentFrame = 0,
            .dyingMaxFrames = 11,
            .dyingTimeToNextFrame = 0.05,
            .dyingFrameTimeCounter = 0
        },
        .baddies = {
            (Baddie) {
                .pos = {
                    .x = BADDIE_COLUMN * GRID_CELL_SIZE,
                    .y = ( BADDIE_LINE - 3 ) * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
                },
                .vel = {
                    .x = 0,
                    .y = 0
                },
                .walkingSpeed = 200,
                .radius = GRID_CELL_SIZE / 2,
                .spriteMap = rm.spriteMap,
                .ySource = 130,
                .color = { 255, 21, 0, 255 },
                .vulnerableColor = { 31, 44, 255, 255 },
                .returningToHuntColor = RAYWHITE,
                .currentFrame = 0,
                .maxFrames = 2,
                .timeToNextFrame = 0.1,
                .frameTimeCounter = 0,
                .direction = DIRECTION_RIGHT,
                .hunting = true,
                .timeToReturnToHunt = TIME_TO_RETURN_TO_HUNT,
                .returnToHuntCounter = 0,
                .timeToStartBlinking = TIME_TO_START_BLINKING,
                .timeToBlink = TIME_TO_BLINK,
                .blinkCounter = 0,
                .blink = false,
                .path = {
                    { 11, 14 }, { 11, 18 }, { 14, 18 }, { 14, 21 }, { 5, 21 }, { 5, 1 }
                },
                .pathSize = 6,
                .currentPathPos = 0,
                .state = ALIVE
            },
            (Baddie) {
                .pos = {
                    .x = ( BADDIE_COLUMN - 2 ) * GRID_CELL_SIZE,
                    .y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
                },
                .vel = {
                    .x = 0,
                    .y = 0
                },
                .walkingSpeed = 200,
                .radius = GRID_CELL_SIZE / 2,
                .spriteMap = rm.spriteMap,
                .ySource = 194,
                .color = { 0, 232, 255, 255 },
                .vulnerableColor = { 31, 44, 255, 255 },
                .returningToHuntColor = RAYWHITE,
                .currentFrame = 0,
                .maxFrames = 2,
                .timeToNextFrame = 0.1,
                .frameTimeCounter = 0,
                .direction = DIRECTION_RIGHT,
                .hunting = true,
                .timeToReturnToHunt = TIME_TO_RETURN_TO_HUNT,
                .returnToHuntCounter = 0,
                .timeToStartBlinking = TIME_TO_START_BLINKING,
                .timeToBlink = TIME_TO_BLINK,
                .blinkCounter = 0,
                .blink = false,
                .path = {
                    { 14, 13 }, { 11, 13 }, { 11, 9 }, { 20, 9 }, { 20, 1 }, { 23, 1 }
                },
                .pathSize = 6,
                .currentPathPos = 0,
                .state = ALIVE
            },
            (Baddie) {
                .pos = {
                    .x = BADDIE_COLUMN * GRID_CELL_SIZE,
                    .y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
                },
                .vel = {
                    .x = 0,
                    .y = 0
                },
                .walkingSpeed = 200,
                .radius = GRID_CELL_SIZE / 2,
                .spriteMap = rm.spriteMap,
                .ySource = 162,
                .color = { 255, 196, 253, 255 },
                .vulnerableColor = { 31, 44, 255, 255 },
                .returningToHuntColor = RAYWHITE,
                .currentFrame = 0,
                .maxFrames = 2,
                .timeToNextFrame = 0.1,
                .frameTimeCounter = 0,
                .direction = DIRECTION_RIGHT,
                .hunting = true,
                .timeToReturnToHunt = TIME_TO_RETURN_TO_HUNT,
                .returnToHuntCounter = 0,
                .timeToStartBlinking = TIME_TO_START_BLINKING,
                .timeToBlink = TIME_TO_BLINK,
                .blinkCounter = 0,
                .blink = false,
                .path = {
                    { 14, 14 }, { 11, 14 }, { 11, 18 }, { 20, 18 }, { 20, 21 }, { 26, 21 }
                },
                .pathSize = 6,
                .currentPathPos = 0,
                .state = ALIVE
            },
            (Baddie) {
                .pos = {
                    .x = ( BADDIE_COLUMN + 2 ) * GRID_CELL_SIZE,
                    .y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
                },
                .vel = {
                    .x = 0,
                    .y = 0
                },
                .walkingSpeed = 200,
                .radius = GRID_CELL_SIZE / 2,
                .spriteMap = rm.spriteMap,
                .ySource = 226,
                .color = { 255, 183, 80, 255 },
                .vulnerableColor = { 31, 44, 255, 255 },
                .returningToHuntColor = RAYWHITE,
                .currentFrame = 0,
                .maxFrames = 2,
                .timeToNextFrame = 0.1,
                .frameTimeCounter = 0,
                .direction = DIRECTION_RIGHT,
                .hunting = true,
                .timeToReturnToHunt = TIME_TO_RETURN_TO_HUNT,
                .returnToHuntCounter = 0,
                .timeToStartBlinking = TIME_TO_START_BLINKING,
                .timeToBlink = TIME_TO_BLINK,
                .blinkCounter = 0,
                .blink = false,
                .path = {
                    { 14, 11 }, { 14, 13 }, { 11, 13 }, { 11, 9 }, { 20, 9 }, { 20, 12 }
                },
                .pathSize = 6,
                .currentPathPos = 0,
                .state = ALIVE
            },
        },
        .boundaryColor = { 35, 44, 218, 255 },
        .doorColor = { 255, 171, 255, 255 },
        .coinColor = { 255, 192, 182, 255 },
        .timeToBlinkBigCoin = 0.2,
        .blinkBigCoinCounter = 0,
        .showBigCoin = true,
        .state = START
    };

    copyTemplateGrid( gw, templateGrid );
    gw->smallCoinRadius = gw->player.radius * 0.3f;
    gw->bigCoinRadius = gw->player.radius * 0.8f;

    for ( int i = 0; i < 4; i++ ) {
        if ( i < BADDIES_TO_RUN ) {
            generateNewPath( &gw->baddies[i], GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE, gw );
        } else {
            break;
        }
    }

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw, float delta ) {

    if ( gw->state == RUNNING ) {

        gw->blinkBigCoinCounter += delta;
        if ( gw->blinkBigCoinCounter > gw->timeToBlinkBigCoin ) {
            gw->blinkBigCoinCounter = 0;
            gw->showBigCoin = !gw->showBigCoin;
        }

        for ( int i = 0; i < 4; i++ ) {
            if ( i < BADDIES_TO_RUN ) {
                updateBaddie( &gw->baddies[i], delta, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE, gw );
            } else {
                break;
            }
        }

        inputAndUpdatePlayer( &gw->player, delta, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE, gw );
        resolvePlayerBaddieCollision( gw, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE );

    } else if ( gw->state == PLAYER_DYING ) {
        inputAndUpdatePlayer( &gw->player, delta, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE, gw );
    }

    if ( IsKeyPressed( KEY_ENTER ) ) {
        if ( gw->state == START || gw->state == PAUSED || gw->player.state == DEAD ) {
            if ( gw->player.state == DEAD ) {
                reset( gw, gw->player.lives == 0 );
            }
            gw->state = RUNNING;
        } else if ( gw->state == RUNNING ) {
            gw->state = PAUSED;
        }
    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    for ( int i = 0; i < GRID_LINES; i++ ) {
        for ( int j = 0; j < GRID_COLUMNS; j++ ) {
            int x = j * GRID_CELL_SIZE;
            int y = i * GRID_CELL_SIZE;
            switch ( gw->grid[i*GRID_COLUMNS+j] ) {
                case O:
                    //DrawRectangle( x, y, GRID_CELL_SIZE, GRID_CELL_SIZE, BLACK );
                    break;
                case M:
                    DrawRectangle( x, y + GRID_CELL_SIZE / 2 - SW / 2, GRID_CELL_SIZE, SW, gw->doorColor );
                    break;
                case H:
                    DrawRectangle( x, y + GRID_CELL_SIZE / 2 - SW / 2, GRID_CELL_SIZE, SW, gw->boundaryColor );
                    break;
                case V:
                    DrawRectangle( x + GRID_CELL_SIZE / 2 - SW / 2, y, SW, GRID_CELL_SIZE, gw->boundaryColor );
                    break;
                case A:
                    DrawRectangle( x + SW * 2, y + SW, SW, SW, gw->boundaryColor );
                    DrawRectangle( x + SW, y + SW * 2, SW, SW, gw->boundaryColor );
                    break;
                case B:
                    DrawRectangle( x, y + SW, SW, SW, gw->boundaryColor );
                    DrawRectangle( x + SW, y + SW * 2, SW, SW, gw->boundaryColor );
                    break;
                case C:
                    DrawRectangle( x + SW * 2, y + SW, SW, SW, gw->boundaryColor );
                    DrawRectangle( x + SW, y, SW, SW, gw->boundaryColor );
                    break;
                case D:
                    DrawRectangle( x, y + SW, SW, SW, gw->boundaryColor );
                    DrawRectangle( x + SW, y, SW, SW, gw->boundaryColor );
                    break;
                case P:
                    //DrawRectangle( x, y, GRID_CELL_SIZE, GRID_CELL_SIZE, BLACK );
                    break;
                case W:
                    DrawCircle( x + GRID_CELL_SIZE / 2, y + GRID_CELL_SIZE / 2, gw->smallCoinRadius, gw->coinColor );
                    break;
                case Q:
                    if ( gw->showBigCoin ) {
                        DrawCircle( x + GRID_CELL_SIZE / 2, y + GRID_CELL_SIZE / 2, gw->bigCoinRadius, gw->coinColor );
                    }
                    break;
            }
        }
    }

    for ( int i = 0; i < 4; i++ ) {
        if ( i < BADDIES_TO_RUN ) {
            drawBaddie( &gw->baddies[i], GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE );
        } else {
            break;
        }
    }

    drawPlayer( &gw->player, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE );

    if ( gw->state == START || gw->player.state == DEAD ) {
        if ( gw->player.lives == 0 ) {
            const char* text = "GAME OVER!";
            DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 40 ) / 2, GetScreenHeight() / 2 + 2, 40, RED );
        } else {
            const char* text = "READY!";
            DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 40 ) / 2, GetScreenHeight() / 2 + 2, 40, gw->player.color );
        }
    } else if ( gw->state == PAUSED ) {
        const char* text = "PAUSED!";
        DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 40 ) / 2, GetScreenHeight() / 2 + 2, 40, gw->player.color );
    }

    if ( DEBUG_GRID ) {
        for ( int i = 0; i <= GRID_LINES; i++ ) {
            DrawLine( 0, i * GRID_CELL_SIZE, GRID_COLUMNS * GRID_CELL_SIZE, i * GRID_CELL_SIZE, WHITE );
        }
        for ( int i = 0; i <= GRID_COLUMNS; i++ ) {
            DrawLine( i * GRID_CELL_SIZE, 0, i * GRID_CELL_SIZE, GRID_LINES * GRID_CELL_SIZE, WHITE );
        }

        for ( int i = 0; i < GRID_LINES; i++ ) {
            for ( int j = 0; j < GRID_COLUMNS; j++ ) {
                DrawText( TextFormat( "%d,%d", i, j ), j * GRID_CELL_SIZE, i * GRID_CELL_SIZE, 10, WHITE );
            }
        }
    }

    EndDrawing();

}

void startHuntingBaddies( GameWorld *gw ) {

    for ( int i = 0; i < 4; i++ ) {
        if ( i < BADDIES_TO_RUN ) {
            gw->baddies[i].hunting = false;
            gw->baddies[i].returnToHuntCounter = 0;
            gw->baddies[i].blinkCounter = 0;
            gw->baddies[i].blink = false;
        } else {
            break;
        }
    }

}

void resolvePlayerBaddieCollision( GameWorld *gw, int lines, int columns, int gridCellSize ) {

    Player *player = &gw->player;
    
    for ( int i = 0; i < 4; i++ ) {
        if ( i < BADDIES_TO_RUN ) {

            Baddie *b = &gw->baddies[i];

            if ( CheckCollisionCircles( player->pos, player->radius, b->pos, b->radius ) ) {

                if ( b->hunting ) {
                    if ( !IMMORTAL ) {
                        gw->state = PLAYER_DYING;
                        player->lives--;
                        player->dyingCurrentFrame = 0;
                        player->dyingFrameTimeCounter = 0;
                        player->state = DYING;
                    }
                } else {
                    TraceLog( LOG_INFO, "captura" );
                    // conta pontos (dobra até finalizar o ciclo de captura)
                    // cria caminho para a casinha, mudando a imagem para os olhinhos
                    // ao chegar na casa, volta ao normal
                }

            }

        } else {
            break;
        }
    }

}

void reset( GameWorld *gw, bool gameOver ) {

    Player *player = &gw->player;
    player->pos = (Vector2) {
        .x = PLAYER_COLUMN * GRID_CELL_SIZE,
        .y = PLAYER_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2
    };
    player->vel.x = 0;
    player->vel.y = 0;
    player->direction = DIRECTION_RIGHT;
    player->state = ALIVE;

    for ( int i = 0; i < 4; i++ ) {
        if ( i < BADDIES_TO_RUN ) {
            Baddie *b = &gw->baddies[i];
            b->hunting = true;
            b->state = ALIVE;
            b->direction = DIRECTION_RIGHT;
            Vector2 pos = { 0 };
            switch ( i ) {
                case 0:
                    pos.x = BADDIE_COLUMN * GRID_CELL_SIZE;
                    pos.y = ( BADDIE_LINE - 3 ) * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
                    break;
                case 1:
                    pos.x = ( BADDIE_COLUMN - 2 ) * GRID_CELL_SIZE;
                    pos.y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
                    break;
                case 2:
                    pos.x = BADDIE_COLUMN * GRID_CELL_SIZE;
                    pos.y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
                    break;
                case 3:
                    pos.x = ( BADDIE_COLUMN + 2 ) * GRID_CELL_SIZE;
                    pos.y = BADDIE_LINE * GRID_CELL_SIZE + GRID_CELL_SIZE / 2;
                    break;
            }
            b->pos = pos;
            generateNewPath( b, GRID_LINES, GRID_COLUMNS, GRID_CELL_SIZE, gw );
        } else {
            break;
        }
    }

    if ( gameOver ) {
        player->lives = 5;
        player->points = 0;
        copyTemplateGrid( gw, templateGrid );
        gw->showBigCoin = true;
    }
    
}

void copyTemplateGrid( GameWorld *gw, const CellType *template ) {
    for ( int i = 0; i < GRID_LINES; i++ ) {
        for ( int j = 0; j < GRID_COLUMNS; j++ ) {
            gw->grid[i*GRID_COLUMNS+j] = template[i*GRID_COLUMNS+j];
        }
    }
}