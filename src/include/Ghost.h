#pragma once

#include <stdbool.h>

#include "Types.h"

#include "raylib/raylib.h"

Ghost createNewGhost( int line, int column, int ySource, const char *name, ChaseTargetType chaseTarget, Quadrant scatterQuadrant, Color color );
void updateGhost( Ghost *ghost, float delta, GameWorld *gw );
void drawGhost( Ghost *ghost );

void generateNewRandomPath( Ghost *ghost, GameWorld *gw );
void generateScatterQuadrantPath( Ghost *ghost, GameWorld *gw );
void generateChaseTargetPath( Ghost *ghost, GameWorld *gw );
void generateBlinkyChasePath( Ghost *ghost, GameWorld *gw );
void generateInkyChasePath( Ghost *ghost, GameWorld *gw );
void generatePinkyChasePath( Ghost *ghost, GameWorld *gw );
void generateClydeChasePath( Ghost *ghost, GameWorld *gw );
void generateNewPath( Ghost *ghost, int targetLine, int targetColumn, GameWorld *gw );
void visitNeightbor( int line, int column, CellPos cell, int *queueSize, int *queueEnd, GameWorld *gw );

void showCapturePoints( Ghost *ghost, CellPos pos, int points );
