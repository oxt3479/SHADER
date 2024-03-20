#include "world.h"

int idToIdx(std::array<int, 3> cell_id) {
    int x = MAX_WORLD_SIZE/2 + cell_id[0];
    int y = MAX_WORLD_SIZE/2 + cell_id[1];
    int z = MAX_WORLD_SIZE/2 + cell_id[2];
    int index = x*MAX_WORLD_SIZE*MAX_WORLD_SIZE + y*MAX_WORLD_SIZE + z;
    if (index >= MAX_WORLD_SIZE*MAX_WORLD_SIZE*MAX_WORLD_SIZE) {
        throw std::runtime_error("Index outside of world limits");
    }
    return index;
}

WorldCell* WorldMap::cellFromID(std::array<int, 3> cell_id) {
    int index = idToIdx(cell_id);
    return cell_grid[index];
};

void WorldMap::addCell(WorldCell& world_cell) {
    int index = idToIdx(world_cell.cell_id);
    cell_grid[index] = &world_cell;
};
