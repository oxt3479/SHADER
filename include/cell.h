#ifndef CELL_H
#define CELL_H

#include "dodecahedron.h"
#include <cmath>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float OPOOP = 1.0f+(1.0f/PHI); // One plue one over phi.. what did you think I wrote?
const float OPSPOOP = (1.0f/(PHI*PHI))+(1.0f/PHI); // Okay this one might even be funnier...
const float OMOOPS = 1.0f-(1.0f/(PHI*PHI));
const float NORMAL_SCALE = 2.0f*OPOOP*OPOOP/sqrt(OPOOP*OPOOP+OPSPOOP*OPSPOOP);
const float DIAGONAL_SCALE = 2.0f*sqrt(OPOOP*OPOOP+OMOOPS*OMOOPS);
const float TWELTH_CORNER_COS = NORMAL_SCALE/DIAGONAL_SCALE;

struct CellSide {
    GLfloat* vertices;
    GLuint* indices;
    int num_faces;
    glm::vec3 findIntercept(glm::vec3 o, glm::vec3 v);
};

struct WorldCell {
    /* The world is built from dodecahedron primitives.... why?
    BECAUSE THEY ARE MAGIC - no furthur elaboration will be provided.*/
    WorldCell();
    WorldCell(WorldCell* parent, int side_idx);
    std::size_t generateVerts(GLfloat* buffer, std::size_t max_size);
    std::size_t generateIndxs(GLuint* buffer, std::size_t max_size);
    bool checkForDoor(int side_idx);
    bool notTooClose(WorldCell* other_cell);
    CellSide sides[12];
    void addDoor(int door_idx, WorldCell& destination);
    glm::vec3 floor_norms[12] = {
        glm::normalize(glm::cross(
            glm::vec3(1, 1, -1) - glm::vec3(1, 1, 1),
            glm::vec3(1+1/PHI, 1-1/pow(PHI, 2), 0) - glm::vec3(1, 1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(1, -1, 1) - glm::vec3(1, 1, 1),
            glm::vec3(1-1/pow(PHI, 2), 0, 1+1/PHI) - glm::vec3(1, 1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, 1, 1) - glm::vec3(1, 1, 1),
            glm::vec3(0, 1+1/PHI, 1-1/pow(PHI, 2)) - glm::vec3(1, 1, 1))),
        -glm::normalize(glm::cross(
            glm::vec3(1, -1, -1) - glm::vec3(1, 1, -1),
            glm::vec3(1-1/pow(PHI, 2), 0, -1-1/PHI) - glm::vec3(1, 1, -1))),
        -glm::normalize(glm::cross(
            glm::vec3(-1, 1, -1) - glm::vec3(1, 1, -1),
            glm::vec3(0, 1+1/PHI, -1+1/pow(PHI, 2)) - glm::vec3(1, 1, -1))),
        -glm::normalize(glm::cross(
            glm::vec3(1, -1, -1) - glm::vec3(1, -1, 1),
            glm::vec3(1+1/PHI, -1+1/pow(PHI, 2), 0) - glm::vec3(1, -1, 1))),
        -glm::normalize(glm::cross(
            glm::vec3(-1, -1, 1) - glm::vec3(1, -1, 1),
            glm::vec3(0, -1-1/PHI, 1-1/pow(PHI, 2)) - glm::vec3(1, -1, 1))),
        -glm::normalize(glm::cross(
            glm::vec3(-1, -1, 1) - glm::vec3(-1, 1, 1),
            glm::vec3(-1+1/pow(PHI, 2), 0, 1+1/PHI) - glm::vec3(-1, 1, 1))),
        -glm::normalize(glm::cross(
            glm::vec3(-1, 1, -1) - glm::vec3(-1, 1, 1),
            glm::vec3(-1-1/PHI, 1-1/pow(PHI, 2), 0) - glm::vec3(-1, 1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, -1, -1) - glm::vec3(1, -1, -1),
            glm::vec3(0, -1-1/PHI, -1+1/pow(PHI, 2)) - glm::vec3(1, -1, -1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, -1, -1) - glm::vec3(-1, -1, 1),
            glm::vec3(-1-1/PHI, -1+1/pow(PHI, 2), 0) - glm::vec3(-1, -1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, -1, -1) - glm::vec3(-1, 1, -1),
            glm::vec3(-1+1/pow(PHI, 2), 0, -1-1/PHI) - glm::vec3(-1, 1, -1)))
    };
    glm::mat4 cell_matrix = glm::mat4(1.0);
    glm::mat4 reflection_mat = glm::mat4(1.0);
    glm::vec3 origin = glm::vec3(0.);
    WorldCell* doors[12] = {NULL};    
};

#endif