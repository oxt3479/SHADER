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

WorldCell::WorldCell(std::array<int, 3> new_cell_id) {
    cell_id = new_cell_id;
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
