#pragma once

#include <stdbool.h>

#include "Types.h"

#include "raylib/raylib.h"

Baddie createNewBaddie( int line, int column, int ySource, Color color );
void updateBaddie( Baddie *baddie, float delta, GameWorld *gw );
void drawBaddie( Baddie *baddie );

void generateNewRandomPath( Baddie *baddie, GameWorld *gw );
void generateNewPath( Baddie *baddie, int targetLine, int targetColumn, GameWorld *gw );
void visitNeightbor( int line, int column, CellPos cell, int *queueSize, int *queueEnd, GameWorld *gw );

void showCapturePoints( Baddie *baddie, Vector2 pos, int points );
