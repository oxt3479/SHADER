#include "spells.h"
#include <iostream>

void teleportA(float progress, glm::vec3 start, glm::vec3 start_up, glm::vec3 target, int target_index, WorldCell* cell, PlayerContext* context){
    static glm::vec3 target_head;
    static glm::vec3 direction;
    target_head = target+cell->floor_norms[target_index]*context->player_location->getHeight();
    direction = normalize(target_head-start)*0.01f;
    context->player_location->teleportHead(mix(target_head, start, progress));
    context->player_location->teleportPUp(mix(cell->floor_norms[target_index], start_up, progress));
};
void teleportAStart(glm::vec3 start, glm::vec3 start_up, glm::vec3 target, int target_index, WorldCell* cell, PlayerContext* context){
    context->player_location->setFloorIndex(target_index);
    context->player_location->reference_cell = cell;
};