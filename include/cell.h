#ifndef CELL_H
#define CELL_H

#include "dodecahedron.h"
#include <cmath>
#include <array>
#include <cstdint>
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
    bool is_door = false;
};

struct WorldCell {
    /* The world is built from dodecahedron primitives.... why?
    BECAUSE THEY ARE MAGIC - no furthur elaboration will be provided.*/
    WorldCell();
    WorldCell(std::array<int, 3> new_cell_id, glm::mat4 new_cell_mat);
    std::array<int, 3> getNeighborID(int side_idx);
    glm::mat4 getNeighborMat(int side_idx);
    CellSide sides[12];
    /*why write it like this? Well otherwise how the fuck do you keep track of 
    what index/vert these normals correspond to, my guy? (actually it was just
    faster for me and now I don't want to change it.)*/
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
        glm::normalize(glm::cross(
            glm::vec3(1, -1, -1) - glm::vec3(1, 1, -1),
            glm::vec3(1-1/pow(PHI, 2), 0, -1-1/PHI) - glm::vec3(1, 1, -1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, 1, -1) - glm::vec3(1, 1, -1),
            glm::vec3(0, 1+1/PHI, -1+1/pow(PHI, 2)) - glm::vec3(1, 1, -1))),
        glm::normalize(glm::cross(
            glm::vec3(1, -1, -1) - glm::vec3(1, -1, 1),
            glm::vec3(1+1/PHI, -1+1/pow(PHI, 2), 0) - glm::vec3(1, -1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, -1, 1) - glm::vec3(1, -1, 1),
            glm::vec3(0, -1-1/PHI, 1-1/pow(PHI, 2)) - glm::vec3(1, -1, 1))),
        glm::normalize(glm::cross(
            glm::vec3(-1, -1, 1) - glm::vec3(-1, 1, 1),
            glm::vec3(-1+1/pow(PHI, 2), 0, 1+1/PHI) - glm::vec3(-1, 1, 1))),
        glm::normalize(glm::cross(
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
    glm::mat4 cell_matrix;

};

#endif