#ifndef DODECAHEDRON_H
#define DODECAHEDRON_H

#ifndef PHI
#define PHI 1.61803f
#endif

#include <glad/glad.h>

struct Dodecahedron {
    GLfloat prim_cell_verts[20*3] = {
        0.0f, 1.0f+1.0f/PHI, 1.0f-1.0f/(PHI*PHI),
        0.0f, -1.0f-1.0f/PHI, -1.0f+1.0f/(PHI*PHI),
        0.0f, -1.0f-1.0f/PHI, 1.0f-1.0f/(PHI*PHI),
        0.0f, 1.0f+1.0f/PHI, -1.0f+1.0f/(PHI*PHI),

        1.0f+1.0f/PHI, 1.0f-1.0f/(PHI*PHI), 0.0f,
        -1.0f-1.0f/PHI, -1.0f+1.0f/(PHI*PHI), 0.0f,
        -1.0f-1.0f/PHI, 1.0f-1.0f/(PHI*PHI), 0.0f,
        1.0f+1.0f/PHI, -1.0f+1.0f/(PHI*PHI), 0.0f,

        1.0f-1.0f/(PHI*PHI), 0.0f, 1.0f+1.0f/PHI,
        -1.0f+1.0f/(PHI*PHI), 0.0f, -1.0f-1.0f/PHI,
        1.0f-1.0f/(PHI*PHI), 0.0f, -1.0f-1.0f/PHI,
        -1.0f+1.0f/(PHI*PHI), 0.0f, 1.0f+1.0f/PHI,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    };

    GLuint prim_cell_indxs[12*9] = {
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

    int adjacency_matrix[12*5] = {
        1, 2, 3, 4, 8,
        0, 2, 3, 5, 6,
        0, 1, 6, 7, 8,
        0, 1, 4, 5, 9,
        0, 3, 8, 9, 11,
        1, 3, 6, 9, 10,
        1, 2, 5, 7, 10,
        2, 6, 8, 10, 11,
        0, 2, 4, 7, 11,
        3, 4, 5, 10, 11,
        5, 6, 7, 9, 11,
        4, 7, 8, 9, 10
    };
};

#endif