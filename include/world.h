#ifndef WORLD_H
#define WORLD_H

#include "cell.h"
#include "bufferObjects.h"
#include <vector>

#define MAX_WORLD_SIZE 32

struct WorldMap {
    WorldCell* cellFromID(std::array<int, 3> cell_id);
    void addCell(WorldCell& world_cell);
    WorldMap();
private:
    WorldCell* cell_grid[MAX_WORLD_SIZE*MAX_WORLD_SIZE*MAX_WORLD_SIZE] = {NULL};
        /* NOTE: technically there is redundancy in the grid, but
        that is because dodecahedrons don't fit perfectly together...*/
};

struct PlayerLocation {    
    PlayerLocation();
    glm::mat4 getView(float x, float y);
    glm::mat4 getModel(std::array<bool, 4> WASD);
    WorldCell* reference_cell = NULL;
    
private:
    float mx = 0.0f;
    float my = 0.0f;
    float height = 0.333f;//... seems good to me
    float movement_scale = 1.0f/1000.0f;
    uint floor_indx;

    glm::vec3 head = glm::vec3(0,0,0);
    glm::vec3 feet = glm::vec3(0,0,-height); 
    glm::vec3 focus = glm::vec3(1,0,0);
        /* NOTE: feet/focus are relative to the head!
        i.e. the actuall coordinates of feet/focus:
        head + feet / head + focus*/
    glm::mat4 Model = glm::mat4(1.0f);

    uint getFloorIndex();
    void updateFocus(float x, float y);
};

struct PlayerContext {
    void linkPlayerCellVAOs();
    void drawPlayerCellVAOs();
    PlayerLocation* player_location = NULL;
    PlayerContext();
private:
    std::vector<WorldCell*> establishNeighborhood();
    WorldMap world_map;
    std::vector<VAO> all_vaos;
};


#endif