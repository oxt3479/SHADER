#ifndef WORLD_H
#define WORLD_H

#include "cell.h"

struct WorldMap {
    WorldCell* cell_grid[MAX_WORLD_SIZE*MAX_WORLD_SIZE*MAX_WORLD_SIZE];
    WorldCell* cellFromID(std::array<int, 3> cell_id);
    void addCell(WorldCell& world_cell);
};

#endif