#include "utils.h"
#include "Types.h"

CellPos getLineAndColumn( Vector2 pos ) {
    return (CellPos) {
        pos.y / GRID_CELL_SIZE,
        pos.x / GRID_CELL_SIZE,
    };
}