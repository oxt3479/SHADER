#include "dodecahedron.h"

GLfloat Dodecahedron::textured_verts[12*5*5]; // 12 sides, 5 corners, xyz uv 
GLuint Dodecahedron::textured_indxs[12*9];

Dodecahedron::Initializer Dodecahedron::initializer;
// Ensures the dodcahedron's textured values are initialized with the following funciton.
// This is all done statically so it does not need to be declared.

void Dodecahedron::populateTexturedVerts() {
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