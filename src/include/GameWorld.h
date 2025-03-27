/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once

#include <stdbool.h>

#include "Types.h"
#include "Player.h"
#include "Baddie.h"

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void );

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw );

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw, float delta );

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw );

void drawGrid( GameWorld *gw );
void drawMessages( GameWorld *gw );

void startHuntingBaddies( GameWorld *gw );
void resolvePlayerBaddieCollision( GameWorld *gw );

void resetGame( GameWorld *gw, bool gameOver );

void copyTemplateGrid( GameWorld *gw, const CellType *template );
