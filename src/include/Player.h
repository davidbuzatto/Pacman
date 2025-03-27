#pragma once

#include "Types.h"

#include "raylib/raylib.h"

Player createNewPlayer( void );
void inputAndUpdatePlayer( Player *player, float delta, GameWorld *gw );
void drawPlayer( Player *player );