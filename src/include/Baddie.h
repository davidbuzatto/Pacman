#pragma once

#include <stdbool.h>

#include "Types.h"
#include "raylib/raylib.h"

void updateBaddie( Baddie *baddie, float delta, int lines, int columns, int gridCellSize, GameWorld *gw );
void drawBaddie( Baddie *baddie, int lines, int columns, int gridCellSize );
Vector2 getBaddieLineAndColumn( Baddie *baddie, int gridCellSize );
void generateNewRandomPath( Baddie *baddie, int lines, int columns, int gridCellSize, GameWorld *gw );
void generateNewPath( Baddie *baddie, int targetLine, int targetColumn, int lines, int columns, int gridCellSize, GameWorld *gw );
void showCapturePoints( Baddie *baddie, Vector2 pos, int points );