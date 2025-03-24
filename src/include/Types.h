#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

#define GRID_LINES 31
#define GRID_COLUMNS 28
#define GRID_CELL_SIZE 24

typedef struct CellPos {
    int line;
    int column;
} CellPos;

typedef enum CellType {
    O,  // obstacle
    M,  // monster door
    H,  // horizontal boundary
    V,  // vertical boundary
    A,  // top-left boundary
    B,  // top-right boundary
    C,  // bottom-left boundary
    D,  // bottom-right boundary
    P,  // path
    W,  // small coin
    Q,  // big coin
} CellType;

typedef enum PlayerDirection {
    PLAYER_DIRECTION_LEFT,
    PLAYER_DIRECTION_RIGHT,
    PLAYER_DIRECTION_UP,
    PLAYER_DIRECTION_DOWN
} PlayerDirection;

typedef struct Player {
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
    PlayerDirection direction;
} Player;

typedef enum BaddieDirection {
    BADDIE_DIRECTION_LEFT,
    BADDIE_DIRECTION_RIGHT,
    BADDIE_DIRECTION_UP,
    BADDIE_DIRECTION_DOWN
} BaddieDirection;

typedef struct Baddie {
    Vector2 pos;
    Vector2 vel;
    float walkingSpeed;
    int radius;
    Texture2D spriteMap;
    int ySource;
    Color color;
    Color vulnerableColor;
    Color returningToHuntColor;
    int currentFrame;
    int maxFrames;
    float timeToNextFrame;
    float frameTimeCounter;
    BaddieDirection direction;
    bool hunting;
    float timeToReturnToHunt;
    float returnToHuntCounter;
    float timeToBlink;
    float timeToStartBlinking;
    float blinkCounter;
    bool blink;
    CellPos path[100];
    int pathSize;
    int currentPathPos;
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
} GameWorld;