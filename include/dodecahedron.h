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
        // right tri, left tri, center tri. (all meet at top)
        // (for passing the primitive directly into shader with no texture)
        4, 12, 0,   4, 13, 3,   4, 0, 3, 
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

    GLfloat texture_corners[5*2] = {
        //temporary approximation...
        0.5f, 0.0f,
        0.0f, 0.4f,
        0.2f, 1.0f,
        0.8f, 1.0f,
        1.0f, 0.4f
    };

    unsigned pentagonal_side_indeces[12*5] = {
        // These wrap around the sides
        // there is a x3 redundancy (3 sides meet per corner)
        4,13,3,0,12,
        8,14,7,4,12,
        0,15,11,8,12,
        10,16,7,4,13,
        3,18,9,10,13,
        7,16,1,2,14,
        2,17,11,8,14,
        11,17,5,6,15,
        6,18,3,0,15,
        1,19,9,10,16,
        5,19,1,2,17,
        9,19,5,6,18
    };

    GLfloat textured_verts[12*5*5] = {
        // 12 sides, 5 corners, xyz uv 
    };

    GLuint textured_indxs[12*9] = {

    };

    constexpr void populateTexturedVerts() {
        int tv = 0, 
            pcv = 0, 
            //(primitive_cell_verts index) 
            tc = 0, 
            //(texture_corners index)
            ti = 0;
            //(texture_indxs index)
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 5; j++) {
                pcv = pentagonal_side_indeces[i*5 + j] * 3;
                tc = j*2;
                textured_verts[tv++] = prim_cell_verts[pcv++];
                textured_verts[tv++] = prim_cell_verts[pcv++];
                textured_verts[tv++] = prim_cell_verts[pcv++];
                textured_verts[tv++] = texture_corners[tc++];
                textured_verts[tv++] = texture_corners[tc++];
            }
            
            textured_indxs[ti++] = i*5+0;
            textured_indxs[ti++] = i*5+1;
            textured_indxs[ti++] = i*5+2;

            textured_indxs[ti++] = i*5+0;
            textured_indxs[ti++] = i*5+2;
            textured_indxs[ti++] = i*5+3;

            textured_indxs[ti++] = i*5+0;
            textured_indxs[ti++] = i*5+3;
            textured_indxs[ti++] = i*5+4;

        }
    }

    int adjacency_matrix[12*5] = {
        // Helps reduce redundant checks with no-adjacent sides
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

    Dodecahedron() {
        populateTexturedVerts();
    }
};

#endif