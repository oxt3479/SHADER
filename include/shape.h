#ifndef SHAPE_H
#define SHAPE_H

#include <glad/glad.h>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/vector_angle.hpp>

#ifndef PHI
#define PHI 1.61803f
#endif

GLfloat vertices[] =
{
    -1.0f,  1.0f, // Top-left
    -1.0f, -1.0f, // Bottom-left
     1.0f, -1.0f, // Bottom-right
     1.0f,  1.0f  // Top-right
};

GLuint indices[] =
{
    0, 1, 2, // First Triangle
    2, 3, 0  // Second Triangle
};

GLfloat triforce_floats[] = { //               COORDINATES                  /     COLORS           //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
};

GLuint triforce_indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
GLfloat cube_vertices[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
GLfloat cube_colors[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

GLfloat dodecahedron_vertices[] = {
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

GLfloat dodecahedron_colors[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
}; // Leaves the 'cube - corners' black (+/-1,+/-1,+/-1) for debug.

GLuint dodecahedron_pentagon_indices[] = {
    12, 13, 4, 0, 3, // top-R, top-L, top, bottom-R, bottom-L (R>0, L<0)
    12, 14, 8, 4, 7,
    12, 15, 0, 8, 11,
    13, 16, 10, 4, 7,
    13, 18, 3, 10, 9,
    14, 16, 7, 2, 1,
    14, 17, 2, 8, 11,
    15, 17, 11, 6, 5,
    15, 18, 6, 0, 3,
    16, 19, 1, 10, 9,
    17, 19, 5, 2, 1,
    18, 19, 9, 6, 5,
};

GLuint dodecahedron_indices[] = {
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

#endif