#include "utils.h"
#include "Types.h"

Vector2 getLineAndColumn( Vector2 pos ) {
    return (Vector2) {
        pos.x / GRID_CELL_SIZE,
        pos.y / GRID_CELL_SIZE,
    };
}