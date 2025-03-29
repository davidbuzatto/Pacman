/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Types.h"
#include "GameWorld.h"
#include "ResourceManager.h"
#include "utils.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raylib/raygui.h"
//#undef RAYGUI_IMPLEMENTATION

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
    H, H, H, H, H, D, W, C, D, P, V, G, G, G,   G, G, G, V, P, C, D, W, C, H, H, H, H, H,
    P, P, P, P, P, P, W, P, P, P, V, G, G, G,   G, G, G, V, P, P, P, W, P, P, P, P, P, P,
    H, H, H, H, H, B, W, A, B, P, V, G, G, G,   G, G, G, V, P, A, B, W, A, H, H, H, H, H,
    O, O, O, O, O, V, W, V, V, P, C, H, H, H,   H, H, H, D, P, V, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, P, P, P, P,   P, P, P, P, P, V, V, W, V, O, O, O, O, O,
    O, O, O, O, O, V, W, V, V, P, A, H, H, H,   H, H, H, B, P, V, V, W, V, O, O, O, O, O,
    A, H, H, H, H, D, W, C, D, P, C, H, H, B,   A, H, H, D, P, C, D, W, C, H, H, H, H, B,
    V, W, W, W, W, W, W, W, W, W, W, W, W, V,   V, W, W, W, W, W, W, W, W, W, W, W, W, V,
    V, W, A, H, H, B, W, A, H, H, H, B, W, V,   V, W, A, H, H, H, B, W, A, H, H, B, W, V,
    V, W, C, H, B, V, W, C, H, H, H, D, W, C,   D, W, C, H, H, H, D, W, V, A, H, D, W, V,
    V, Q, W, W, V, V, W, W, W, W, W, W, W, P,   P, W, W, W, W, W, W, W, V, V, W, W, Q, V, // Q test (if needed)
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
        .pacman = createNewPacman( 23, 14 ),
        .ghosts = {
            createNewGhost( 
                11, 14, 130, 
                "Blinky", 
                CHASE_TARGET_TYPE_BLINKY, (Quadrant){ 1, 21, 5, 26 },
                (Color){ 255, 21, 0, 255 }
            ),
            createNewGhost( 
                14, 12, 194, 
                "Inky",
                CHASE_TARGET_TYPE_INKY, (Quadrant){ 25, 21, 29, 26 },
                (Color){ 0, 232, 255, 255 }
            ),
            createNewGhost( 
                14, 14, 162,
                "Pinky",
                CHASE_TARGET_TYPE_PINKY, (Quadrant){ 1, 1, 5, 6 },
                (Color){ 255, 196, 253, 255 }
            ),
            createNewGhost( 
                14, 16, 226, 
                "Clyde", 
                CHASE_TARGET_TYPE_CLYDE, (Quadrant){ 25, 1, 29, 6 },
                (Color){ 255, 183, 80, 255 }
            )
        },
        .boundaryColor = { 35, 44, 218, 255 },
        .doorColor = { 255, 171, 255, 255 },
        .coinColor = { 255, 192, 182, 255 },

        .smallCoinRadius = 0,
        .bigCoinRadius = 0,
        .timeToBlinkBigCoin = 0.2,
        .blinkBigCoinCounter = 0,
        .showBigCoin = true,

        .ghostCaptureBasePoints = 200,
        .ghostCaptureCurrentPoints = 200,
        .remainingCoins = 0,

        .marked = { 0 },
        .edgeTo = { 0 },
        .queue = { 0 },

        .state = START

    };

    gw->smallCoinRadius = gw->pacman.radius * 0.3f;
    gw->bigCoinRadius = gw->pacman.radius * 0.8f;

    copyTemplateGrid( gw, templateGrid );
    
    for ( int i = 0; i < 4; i++ ) {
        generateChaseTargetPath( &gw->ghosts[i], gw );
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

    if ( IsKeyPressed( KEY_ENTER ) ) {
        if ( gw->state == START || gw->state == PAUSED || gw->state == WON || gw->pacman.state == DEAD ) {
            bool gameOver = gw->pacman.lives == 0;
            if ( gw->state == WON || gw->pacman.state == DEAD ) {
                resetGame( gw, gw->state == WON || gameOver );
            }
            if ( gw->state == WON || gameOver ) {
                gw->state = START;
            } else {
                gw->state = RUNNING;
            }
        } else if ( gw->state == RUNNING ) {
            gw->state = PAUSED;
        }
    }
    
    if ( gw->remainingCoins == 0 ) {
        gw->state = WON;
    }

    if ( gw->state == RUNNING ) {

        gw->blinkBigCoinCounter += delta;
        if ( gw->blinkBigCoinCounter > gw->timeToBlinkBigCoin ) {
            gw->blinkBigCoinCounter = 0;
            gw->showBigCoin = !gw->showBigCoin;
        }

        for ( int i = 0; i < 4; i++ ) {
            updateGhost( &gw->ghosts[i], delta, gw );
        }

        inputAndUpdatePacman( &gw->pacman, delta, gw );
        resolvePacmanGhostCollision( gw );

    } else if ( gw->state == PLAYER_DYING ) {
        inputAndUpdatePacman( &gw->pacman, delta, gw );
    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    drawGrid( gw );

    for ( int i = 0; i < 4; i++ ) {
        drawGhost( &gw->ghosts[i] );
    }

    drawPacman( &gw->pacman );
    drawMessages( gw ) ;

    if ( DRAWING_DEBUG ) {

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

void drawGrid( GameWorld *gw ) {

    int squareWidth = GRID_CELL_SIZE / 3;

    for ( int i = 0; i < GRID_LINES; i++ ) {
        for ( int j = 0; j < GRID_COLUMNS; j++ ) {
            int x = j * GRID_CELL_SIZE;
            int y = i * GRID_CELL_SIZE;
            switch ( gw->grid[i*GRID_COLUMNS+j] ) {
                case O:
                    break;
                case H:
                    DrawRectangle( x, y + GRID_CELL_SIZE / 2 - squareWidth / 2, GRID_CELL_SIZE, squareWidth, gw->boundaryColor );
                    break;
                case V:
                    DrawRectangle( x + GRID_CELL_SIZE / 2 - squareWidth / 2, y, squareWidth, GRID_CELL_SIZE, gw->boundaryColor );
                    break;
                case A:
                    DrawRectangle( x + squareWidth * 2, y + squareWidth, squareWidth, squareWidth, gw->boundaryColor );
                    DrawRectangle( x + squareWidth, y + squareWidth * 2, squareWidth, squareWidth, gw->boundaryColor );
                    break;
                case B:
                    DrawRectangle( x, y + squareWidth, squareWidth, squareWidth, gw->boundaryColor );
                    DrawRectangle( x + squareWidth, y + squareWidth * 2, squareWidth, squareWidth, gw->boundaryColor );
                    break;
                case C:
                    DrawRectangle( x + squareWidth * 2, y + squareWidth, squareWidth, squareWidth, gw->boundaryColor );
                    DrawRectangle( x + squareWidth, y, squareWidth, squareWidth, gw->boundaryColor );
                    break;
                case D:
                    DrawRectangle( x, y + squareWidth, squareWidth, squareWidth, gw->boundaryColor );
                    DrawRectangle( x + squareWidth, y, squareWidth, squareWidth, gw->boundaryColor );
                    break;
                case G:
                    break;
                case M:
                    DrawRectangle( x, y + GRID_CELL_SIZE / 2 - squareWidth / 2, GRID_CELL_SIZE, squareWidth, gw->doorColor );
                    break;
                case P:
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

}

void drawMessages( GameWorld *gw ) {

    if (  gw->state == WON ) {
        const char* text = "CONGRATULATIONS!";
        DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.7f ) );
        DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 60 ) / 2, GetScreenHeight() / 2 - 80, 60, GREEN );
    } else if ( gw->state == START || gw->pacman.state == DEAD ) {
        if ( gw->pacman.lives == 0 ) {
            const char* text = "GAME OVER!";
            DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), Fade( BLACK, 0.7f ) );
            DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 60 ) / 2, GetScreenHeight() / 2 - 80, 60, RED );
        } else {
            const char* text = "READY!";
            DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 40 ) / 2, GetScreenHeight() / 2 + 2, 40, gw->pacman.color );
        }
    } else if ( gw->state == PAUSED ) {
        const char* text = "PAUSED!";
        DrawText( text, GetScreenWidth() / 2 - MeasureText( text, 40 ) / 2, GetScreenHeight() / 2 + 2, 40, gw->pacman.color );
    }

}

void startScatterGhosts( GameWorld *gw ) {

    bool resetCaptureCurrentPoints = false;

    for ( int i = 0; i < 4; i++ ) {

        Ghost *g = &gw->ghosts[i];

        if ( !resetCaptureCurrentPoints && g->chasing ) {
            resetCaptureCurrentPoints = true;
        }

        g->returnToChaseCounter = 0;
        g->blinkCounter = 0;
        g->blink = false;

        if ( g->chasing && g->state != RETURNING_HOME ) {

            g->chasing = false;
            
            g->currentPathPos = -1;
            g->vel.x = 0;
            g->vel.y = 0;
            generateScatterQuadrantPath( g, gw );

        }

    }

    if ( resetCaptureCurrentPoints ) {
        gw->ghostCaptureCurrentPoints = gw->ghostCaptureBasePoints;
    }

}

void resolvePacmanGhostCollision( GameWorld *gw ) {

    Pacman *pacman = &gw->pacman;
    
    for ( int i = 0; i < 4; i++ ) {

        Ghost *g = &gw->ghosts[i];

        if ( CheckCollisionCircles( pacman->pos, pacman->radius, g->pos, g->radius ) ) {

            if ( g->chasing ) {
                if ( !IMMORTAL && g->state != RETURNING_HOME ) {
                    gw->state = PLAYER_DYING;
                    pacman->lives--;
                    pacman->dyingCurrentFrame = 0;
                    pacman->dyingFrameTimeCounter = 0;
                    pacman->state = DYING;
                }
            } else if ( g->state != RETURNING_HOME ) {
                g->state = RETURNING_HOME;
                g->currentPathPos = -1;
                g->vel.x = 0;
                g->vel.y = 0;
                pacman->points += gw->ghostCaptureCurrentPoints;
                showCapturePoints( g, getLineAndColumn( g->pos ), gw->ghostCaptureCurrentPoints );
                gw->ghostCaptureCurrentPoints *= 2;
                generateNewPath( g, 14, GetRandomValue( 13, 14 ), gw );
            }

        }

    }

}

void resetGame( GameWorld *gw, bool gameOver ) {

    Pacman *pacman = &gw->pacman;
    pacman->pos = pacman->startPos;
    pacman->vel.x = 0;
    pacman->vel.y = 0;
    pacman->direction = DIRECTION_RIGHT;
    pacman->state = ALIVE;
    pacman->currentFrame = 0;

    for ( int i = 0; i < 4; i++ ) {
        Ghost *g = &gw->ghosts[i];
        g->chasing = true;
        g->state = ALIVE;
        g->direction = DIRECTION_RIGHT;
        g->pos = g->startPos;
        generateChaseTargetPath( g, gw );
    }

    if ( gameOver ) {
        pacman->lives = 5;
        pacman->points = 0;
        copyTemplateGrid( gw, templateGrid );
        gw->showBigCoin = true;
    }
    
}

void copyTemplateGrid( GameWorld *gw, const CellType *template ) {

    int coins = 0;

    for ( int i = 0; i < GRID_LINES; i++ ) {
        for ( int j = 0; j < GRID_COLUMNS; j++ ) {
            gw->grid[i*GRID_COLUMNS+j] = template[i*GRID_COLUMNS+j];
            if ( gw->grid[i*GRID_COLUMNS+j] == W || gw->grid[i*GRID_COLUMNS+j] == Q ) {
                coins++;
            }
        }
    }

    gw->remainingCoins = coins;

}