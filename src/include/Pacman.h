#pragma once

#include "Types.h"

#include "raylib/raylib.h"

Pacman createNewPacman( int line, int column );
void inputAndUpdatePacman( Pacman *pacman, float delta, GameWorld *gw );
void drawPacman( Pacman *pacman );