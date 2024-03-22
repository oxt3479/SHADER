#include "cell.h"

using namespace glm;

WorldCell::WorldCell() {
    /*Simplest possible world cell initialization...,
    assumes building directly from primitve. 3 triangels per
    pentagon means +=9*/
    GLuint* cell_indxs_ptr = cell_indxs;
    for (uint i=0; i < 12; i++) {
        sides[i].vertices = cell_verts;
        sides[i].indices  = cell_indxs_ptr;
        sides[i].num_faces = 3;

        cell_indxs_ptr+=9;
    }
    cell_id = {0};
    cell_matrix = mat4(1.0);
};

WorldCell::WorldCell(std::array<int, 3> new_cell_id, glm::mat4 new_cell_mat) {
    /*TODO : combine this with the alternateive initialization above somehow (redundant?)*/
    GLuint* cell_indxs_ptr = cell_indxs;
    for (uint i=0; i < 12; i++) {
        sides[i].vertices = cell_verts;
        sides[i].indices  = cell_indxs_ptr;
        sides[i].num_faces = 3;

        cell_indxs_ptr+=9;
    }
    cell_id = new_cell_id;
    cell_matrix = new_cell_mat;
};

std::array<int, 3> WorldCell::getNeighborID(int side_idx) {
    std::array<int, 3> id_array;
    switch (side_idx) {
        case 0: id_array = {1, 1, 0};   break;
        case 1: id_array = {1, 0, 1};   break;
        case 2: id_array = {0, 1, 1};   break;
        case 3: id_array = {-1, 0, 1};  break;
        case 4: id_array = {0, -1, 1};  break;
        case 5: id_array = {-1, 1, 0};  break;
        case 6: id_array = {0, 1, -1};  break;
        case 7: id_array = {1, 0, -1};  break;
        case 8: id_array = {1, -1, 0};  break;
        case 9: id_array = {0, -1, -1}; break;
        case 10:id_array = {-1, -1, 0}; break;
        case 11:id_array = {-1, 0, -1}; break;
        default:id_array = {0};
            throw std::runtime_error("Impossible side requested for id");
    }
    for (int i = 0; i < 3; i++) id_array[i] = id_array[i] + cell_id[i];
        // Id is offset from the current cell's id.
    return id_array;
};

glm::mat4 WorldCell::getNeighborMat(int side_idx) {
    /*Since all of these are always the same for each index this could technically
    be replaced with precalculated matricies...*/
    vec3 n = floor_norms[side_idx];
    glm::mat4 new_mat = mat4(
        vec4(-2.0f*n.x*n.x+1.0f,     -2.0f*n.y*n.x,      -2.0f*n.z*n.x,      0.0f),
        vec4(-2.0f*n.x*n.y,          -2.0f*n.y*n.y+1.0f, -2.0f*n.z*n.y,      0.0f),
        vec4(-2.0f*n.z*n.x,          -2.0f*n.y*n.z,      -2.0f*n.z*n.z+1.0f, 0.0f),
        vec4(0.0f,                   0.0f,               0.0f,               1.0f)
    );// This matrix reflects over the corresponding normal...
    float OPOOP = 1.0f+(1.0f/PHI); // One plue one over phi.. what did you think I wrote?
    float OPSPOOP = (1.0f/(PHI*PHI))+(1.0f/PHI); // Okay this one might even be funnier...

    n *= OPOOP*OPOOP/sqrt(OPOOP*OPOOP+OPSPOOP*OPSPOOP);
    /*This mess makes n, already normal to the surface, also
    touch the center of the face, from the origin. */
    new_mat = new_mat*translate(cell_matrix, 2.0f*n);
    return new_mat;
};

bool checkTriangle(vec3 v, vec3 p1, vec3 p2, vec3 p3,
                    float& a, float& b, float& c) {
    /*Assumes ray is coming from the origin. 
    Which it is for the head of the player... if your using it right
    https://gamedev.stackexchange.com/questions/114955/m%C3%B6ller-trumbore-intersection-point*/
    
    const float EPSILON = 0.0000001f;
    vec3 e1 = p2 - p1;
    vec3 e2 = p3 - p1;
    vec3 vce2 = cross(v, e2);

    float det = dot(e1, vce2);

    if (det < EPSILON && det > -EPSILON) return false;

    float idet = 1.0f/det;
    //vec3 tvec = origin-p1; //origin is 0 here... just fyi..
    b = dot(-p1, vce2) * idet;
    if (b < 0 || b > 1) return false;

    vec3 qvec = cross(-p1, e1);
    c = dot(v, qvec) * idet;
    if (c < 0 || b+c > 1) return false;

    a = dot(e2, qvec) * idet;
    return true;
    
};

vec3 CellSide::findIntercept(vec3 v) {
    /* TODO:  This could be done on the GPU w CUDA or datashaders...*/
    float a,b,c;
    uint inda,indb,indc;
    vec3 p1, p2, p3;
    for (uint i=0; i < num_faces; i++) {
        /* Consider : saving the last working index, and starting from there,
        and looping around? (wouldn't work on GPU?) */
        inda = indices[i*3];
        indb = indices[i*3+1];
        indc = indices[i*3+2];
        p1 = vec3(vertices[inda*3], vertices[inda*3+1], vertices[inda*3+2]);
        p2 = vec3(vertices[indb*3], vertices[indb*3+1], vertices[indb*3+2]);
        p3 = vec3(vertices[indc*3], vertices[indc*3+1], vertices[indc*3+2]);
        if ( checkTriangle(v, p1, p2, p3, a,b,c) ) {
            return v*a;
        }
    }
    throw std::runtime_error("ERROR : no intersection found.");
    return v;
};
