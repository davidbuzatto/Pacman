#pragma once

#include "Types.h"
#include "raylib/raylib.h"

void inputAndUpdatePlayer( Player *player, float delta, int lines, int columns, int gridCellSize, GameWorld *gw );
void drawPlayer( Player *player, int lines, int columns, int gridCellSize );
Vector2 getPlayerLineAndColumn( Player *player, int gridCellSize );
Vector2 getLineAndColumnFromPos( Vector2 pos, int gridCellSize );
