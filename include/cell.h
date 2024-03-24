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

struct CellSide {
    GLfloat* vertices;
    GLuint* indices;
    int num_faces;
    glm::vec3 findIntercept(glm::vec3 v);
};

struct WorldCell {
    /* The world is built from dodecahedron primitives.... why?
    BECAUSE THEY ARE MAGIC - no furthur elaboration will be provided.*/
    WorldCell();
    WorldCell(std::array<int, 3> new_cell_id, WorldCell* parent, int side_idx);
    std::size_t generateVerts(GLfloat* buffer, std::size_t max_size);
    std::size_t generateIndxs(GLuint* buffer, std::size_t max_size);
    std::array<int, 3> getNeighborID(int side_idx);
    glm::mat4 getNeighborMat(int side_idx);
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
    std::array<int, 3> cell_id;
    glm::mat4 cell_matrix = glm::mat4(1.0);
    glm::mat4 reflection_mat = glm::mat4(1.0);
private:
    WorldCell* doors[12] = {NULL};
    glm::vec3 origin = glm::vec3(0.);
};

#endif