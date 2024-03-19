#include "world.h"

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

vec3 WorldSide::findIntercept(vec3 v) {
    /* TODO:  This could be done on the GPU w CUDA or datashaders...*/
    float a,b,c;
    uint inda,indb,indc;
    vec3 p1, p2, p3;
    for (uint i=0; i < num_faces; i++) {
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

PlayerLocation::PlayerLocation(float * start_time) {
    floor_indx = getFloorIndex(); 
        // Technically, you can choose any int 0-12
    const vec3* up          = &reference_cell->floor_norms[floor_indx];
    WorldSide* floor_mesh   = &reference_cell->sides[floor_indx];
    vec3 floor              = floor_mesh->findIntercept(-*up);

    feet = floor;
    head = floor + (*up)*height;
    time = start_time;
    
    Model = translate(Model, -head);
        // update model matrix with the head
};
mat4 PlayerLocation::getView(float x, float y) {
    updateFocus(x, y); // Account for mouse movement...
    mat4 View = lookAt(
        vec3(0), focus, -feet
    );// since this is view, keep the origin at 0, and not head...
    return View;
};
mat4 PlayerLocation::getModel() {    
    /* here we account for the head's displacement... not sure if having
    the mat4s seperated is helpful... TODO*/
    return Model;
};
uint PlayerLocation::getFloorIndex() {
    /*Given arbitrary head/feet this gives the closest aligned
    surface normal in the dodecahedron defining the cell*/
    float min = 0.0;
    int min_idx = 0;
    for (unsigned i; i < 12; i++) {
        float product = dot(reference_cell->floor_norms[i], head-feet);
        if (product < min) {
            min = product;
            min_idx = i;
        }
    }
    return min_idx;
};
void PlayerLocation::updateFocus(float x, float y) {
    /*For now this is 'normalized', but hypothetically,
    focus could permit DOF changes...*/
    vec3 vertical   = normalize(feet);
    vec3 horizontal = normalize(cross(vertical, focus));
    
    float dx = (x-mx < 0.1) ? x-mx : 0.1;
    float dy = (y-my < 0.1) ? y-my : 0.1;

    focus = normalize(focus) + (dx*horizontal) + (dy*vertical);

    mx = x;
    my = y;
};
/*TODO: Use more modern "track ball" approach? 
(Current implementation behaves weird lookging up or down too far)
https://github.com/RockefellerA/Trackball/blob/master/Trackball.cpp*/