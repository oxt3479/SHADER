#include "dodecahedron.h"

// Define all static members
GLfloat Dodecahedron::prim_cell_verts[20*3];
GLuint Dodecahedron::prim_cell_indxs[12*9];
GLfloat Dodecahedron::texture_corners[5*2];
unsigned Dodecahedron::pentagonal_side_indeces[12*5];
int Dodecahedron::adjacency_matrix[12*5];

GLfloat Dodecahedron::textured_verts[12*5*5];
GLuint Dodecahedron::textured_indxs[12*9];

// Initialize the static Initializer
Dodecahedron::Initializer Dodecahedron::initializer;