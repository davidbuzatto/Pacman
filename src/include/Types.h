#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

#define GRID_LINES 31
#define GRID_COLUMNS 28
#define GRID_CELL_SIZE 24

typedef enum CellType {
    O,  // obstacle
    H,  // horizontal boundary
    V,  // vertical boundary
    A,  // top-left boundary
    B,  // top-right boundary
    C,  // bottom-left boundary
    D,  // bottom-right boundary
    P,  // path
    M,  // monster door
    W,  // small coin
    Q,  // big coin
} CellType;

typedef enum State {
    START,
    RUNNING,
    PAUSED,
    PLAYER_DYING,
    ALIVE,
    DYING,
    DEAD
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
    Direction direction;
    State state;
    int dyingCurrentFrame;
    int dyingMaxFrames;
    float dyingTimeToNextFrame;
    float dyingFrameTimeCounter;
} Player;

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
    Direction direction;
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
    State state;
} GameWorld;