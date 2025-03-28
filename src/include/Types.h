#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

#define GRID_LINES 31
#define GRID_COLUMNS 28
#define GRID_CELL_SIZE 24

typedef enum CellType {

    // boundaries and obstacles
    O,  // internal space
    H,  // horizontal boundary
    V,  // vertical boundary
    A,  // top-left boundary
    B,  // top-right boundary
    C,  // bottom-left boundary
    D,  // bottom-right boundary

    // only accessible by ghosts
    G,  // ghost house
    M,  // monster door

    // acessible by pacman and ghosts
    P,  // path
    W,  // small coin
    Q,  // big coin

} CellType;

typedef enum State {

    // game states
    START,
    RUNNING,
    PAUSED,
    WON,
    PLAYER_DYING,

    // pacman and ghost states
    ALIVE,
    DYING,
    DEAD,
    RETURNING_HOME

} State;

typedef enum Direction {
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_DOWN
} Direction;

typedef enum ChaseTargetType {
    CHASE_TARGET_TYPE_BLINKY,
    CHASE_TARGET_TYPE_INKY,
    CHASE_TARGET_TYPE_PINKY,
    CHASE_TARGET_TYPE_CLYDE
} ChaseTargetType;

typedef struct CellPos {
    int line;
    int column;
} CellPos;

typedef struct Quadrant {
    int line1;
    int column1;
    int line2;
    int column2;
} Quadrant;

typedef struct Pacman {

    Vector2 startPos;
    Vector2 pos;
    Vector2 vel;
    float walkingSpeed;
    int radius;
    Color color;

    int lives;
    int points;

    int currentFrame;
    int maxFrames;
    float timeToNextFrame;
    float frameTimeCounter;

    int dyingCurrentFrame;
    int dyingMaxFrames;
    float dyingTimeToNextFrame;
    float dyingFrameTimeCounter;

    Direction direction;
    State state;

} Pacman;

typedef struct Ghost {

    Vector2 startPos;
    Vector2 pos;
    Vector2 vel;
    float chasingSpeed;
    float scatterSpeed;
    float returningHomeSpeed;
    int radius;
    Texture2D spriteMap;
    int ySource;
    Color color;
    const char *name;

    int currentFrame;
    int maxFrames;
    float timeToNextFrame;
    float frameTimeCounter;

    bool chasing;
    float timeToReturnToChase;
    float returnToChaseCounter;

    bool blink;
    float timeToStartBlinking;
    float timeToBlink;
    float blinkCounter;

    CellPos path[100];
    int pathSize;
    int currentPathPos;

    bool showCapturePoints;
    Vector2 capturePointsPos;
    int capturePoints;
    float timeToShowPoints;
    float showPointsCounter;

    ChaseTargetType chaseTarget;
    Quadrant scatterQuadrant;

    Direction direction;
    State state;

} Ghost;

typedef struct GameWorld {

    CellType grid[GRID_LINES*GRID_COLUMNS];
    Pacman pacman;
    Ghost ghosts[4];

    Color boundaryColor;
    Color doorColor;
    Color coinColor;

    float smallCoinRadius;
    float bigCoinRadius;
    float timeToBlinkBigCoin;
    float blinkBigCoinCounter;
    bool showBigCoin;
    
    int ghostCaptureBasePoints;
    int ghostCaptureCurrentPoints;
    int remainingCoins;

    // BFS data structures
    bool marked[GRID_LINES][GRID_COLUMNS];
    CellPos edgeTo[GRID_LINES][GRID_COLUMNS];
    CellPos queue[GRID_LINES*GRID_COLUMNS];

    State state;

} GameWorld;