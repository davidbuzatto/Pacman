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

    // only accessible by baddies
    G,  // ghost house
    M,  // monster door

    // acessible by player and baddies
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

    // player and baddie states
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

typedef struct CellPos {
    int line;
    int column;
} CellPos;

typedef struct Player {

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

} Player;

typedef struct Baddie {

    Vector2 startPos;
    Vector2 pos;
    Vector2 vel;
    float walkingSpeed;
    int radius;
    Texture2D spriteMap;
    int ySource;
    Color color;

    int currentFrame;
    int maxFrames;
    float timeToNextFrame;
    float frameTimeCounter;

    bool hunting;
    float timeToReturnToHunt;
    float returnToHuntCounter;

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

    Direction direction;
    State state;

} Baddie;

typedef struct GameWorld {

    CellType grid[GRID_LINES*GRID_COLUMNS];
    Player player;
    Baddie baddies[4];

    Color boundaryColor;
    Color doorColor;
    Color coinColor;

    float smallCoinRadius;
    float bigCoinRadius;
    float timeToBlinkBigCoin;
    float blinkBigCoinCounter;
    bool showBigCoin;
    
    int baddieCaptureBasePoints;
    int baddieCaptureCurrentPoints;
    int remainingCoins;

    // BFS data structures
    bool marked[GRID_LINES][GRID_COLUMNS];
    CellPos edgeTo[GRID_LINES][GRID_COLUMNS];
    CellPos queue[GRID_LINES*GRID_COLUMNS];

    State state;

} GameWorld;