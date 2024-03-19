#ifndef WORLD_H
#define WORLD_H

#ifndef PHI
#define PHI 1.61803f
#endif

#include <cmath>
#include <array>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct WorldSide {
    GLfloat* vertices;
    GLuint* indices;
    int num_faces;
    glm::vec3 findIntercept(glm::vec3 v);
};

struct WorldCell {
    /* The world is built from dodecahedron primitives.... why?
    BECAUSE THEY ARE MAGIC - no furthur elaboration will be provided.*/
    WorldCell();
    WorldSide sides[12];    
    GLfloat cell_verts[20*3] = {
        0.0f, 1.0f+1.0f/PHI, 1.0f-1.0f/pow(PHI, 2),
        0.0f, -1.0f-1.0f/PHI, -1.0f+1.0f/pow(PHI, 2),
        0.0f, -1.0f-1.0f/PHI, 1.0f-1.0f/pow(PHI, 2),
        0.0f, 1.0f+1.0f/PHI, -1.0f+1.0f/pow(PHI, 2),

        1.0f+1.0f/PHI, 1.0f-1.0f/pow(PHI, 2), 0.0f,
        -1.0f-1.0f/PHI, -1.0f+1.0f/pow(PHI, 2), 0.0f,
        -1.0f-1.0f/PHI, 1.0f-1.0f/pow(PHI, 2), 0.0f,
        1.0f+1.0f/PHI, -1.0f+1.0f/pow(PHI, 2), 0.0f,

        1.0f-1.0f/pow(PHI, 2), 0.0f, 1.0f+1.0f/PHI,
        -1.0f+1.0f/pow(PHI, 2), 0.0f, -1.0f-1.0f/PHI,
        1.0f-1.0f/pow(PHI, 2), 0.0f, -1.0f-1.0f/PHI,
        -1.0f+1.0f/pow(PHI, 2), 0.0f, 1.0f+1.0f/PHI,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    };
    GLuint cell_indxs[12*9] = {
        4, 12, 0,   4, 13, 3,   4, 0, 3, // right tri, left tri, center tri. (all meet at top)
        8, 12, 4,   8, 14, 7,   8, 4, 7,
        0, 12, 8,   0, 15, 11,  0, 8, 11,
        10, 13, 4,  10, 16, 7,  10, 4, 7,
        3, 13, 10,  3, 18, 9,   3, 10, 9,
        7, 14, 2,   7, 16, 1,   7, 2, 1,
        2, 14, 8,   2, 17, 11,  2, 8, 11,
        11, 15, 6,  11, 17, 5,  11, 6, 5,
        6, 15, 0,   6, 18, 3,   6, 0, 3,
        1, 16, 10,  1, 19, 9,   1, 10, 9,
        5, 17, 2,   5, 19, 1,   5, 2, 1,
        9, 18, 6,   9, 19, 5,   9, 6, 5
    };
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
};

class PlayerLocation {
public:
    
    PlayerLocation(float * time);
    glm::mat4 getView(float x, float y);
    glm::mat4 getModel();
    WorldCell* reference_cell = new WorldCell();
    
private:
    float * time;
    float mx = 0.0f;
    float my = 0.0f;
    float height = 0.333f;//... seems good to me
    uint floor_indx;

    glm::vec3 head = glm::vec3(0,0,0);
    glm::vec3 feet = glm::vec3(0,0,-height); 
    glm::vec3 focus = glm::vec3(1,0,0);
    glm::mat4 Model = glm::mat4(1.0f);

    uint getFloorIndex();
    void updateFocus(float x, float y);
};

#endif