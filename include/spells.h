#ifndef SPELLS_H
#define SPELLS_H

#include "world.h"

void teleportA(float progress, glm::vec3 start, glm::vec3 start_up, glm::vec3 target, int target_index, WorldCell* cell, PlayerContext* context);
void teleportAStart(glm::vec3 start, glm::vec3 start_up, glm::vec3 target, int target_index, WorldCell* cell, PlayerContext* context);
#endif