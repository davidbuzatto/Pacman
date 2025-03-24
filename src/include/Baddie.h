#pragma once

#include <stdbool.h>

#include "Types.h"
#include "raylib/raylib.h"

void updateBaddie( Baddie *baddie, float delta, int lines, int columns, int gridCellSize, GameWorld *gw );
void drawBaddie( Baddie *baddie, int lines, int columns, int gridCellSize );
Vector2 getBaddieLineAndColumn( Baddie *baddie, int gridCellSize );
Vector2 getLineAndColumnFromPosBaddie( Vector2 pos, int gridCellSize );
void generateNewPath( Baddie *baddie, int lines, int columns, int gridCellSize, GameWorld *gw );