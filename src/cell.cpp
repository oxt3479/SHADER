#include "cell.h"
#include <glm/gtx/string_cast.hpp>
using namespace glm;

Dodecahedron ref_dodecahedron;
/* bool isReflected(std::array<int, 3> cell_id) {
    bool is_reflected = false;
    //Depending on whether the current WorldCell is a reflection of 
    //the original or not, we will need to calculate the new id differently.
    for (int i = 0; i < 3; i++) {
        is_reflected = cell_id[i]%2 != 0;
        if (is_reflected) break;
    }
    return is_reflected;
}; */
void buildSides(WorldCell* world_cell) {
    /*Simplest possible world cell initialization...,
    assumes building directly from primitve. 3 triangels per
    pentagon means +=9 
    TODO: this may be rundnadant if ref_dodecahedron 
    is always the same...*/
    GLuint* cell_indxs_ptr = ref_dodecahedron.prim_cell_indxs;
    for (uint i=0; i < 12; i++) {
        world_cell->sides[i].vertices = ref_dodecahedron.prim_cell_verts;
        world_cell->sides[i].indices  = cell_indxs_ptr;
        world_cell->sides[i].num_faces = 3;
        cell_indxs_ptr+=9;
    }
}
WorldCell::WorldCell() {
    buildSides(this);
    cell_id = {0};
    cell_matrix = mat4(1.0);
    origin = vec3(0.0);
};
const float OPOOP = 1.0f+(1.0f/PHI); // One plue one over phi.. what did you think I wrote?
const float OPSPOOP = (1.0f/(PHI*PHI))+(1.0f/PHI); // Okay this one might even be funnier...
const float NORMAL_SCALE = 2.0f*OPOOP*OPOOP/sqrt(OPOOP*OPOOP+OPSPOOP*OPSPOOP);
vec3 getNeighborOffset(vec3 n) {
    /*This mess makes n, already normal to the surface, also
    touch the center of the face, from `this` origin to
    the `neighbor` origin */
    return -n * NORMAL_SCALE;
};
mat4 getReflectionMatrix(vec3 n) {
    /*Gives a matrix that reflects over the corresponding
    side's normal*/
    return mat4(
        vec4(-2.0f*n.x*n.x+1.0f,     -2.0f*n.y*n.x,      -2.0f*n.z*n.x,      0.0f),
        vec4(-2.0f*n.x*n.y,          -2.0f*n.y*n.y+1.0f, -2.0f*n.z*n.y,      0.0f),
        vec4(-2.0f*n.z*n.x,          -2.0f*n.y*n.z,      -2.0f*n.z*n.z+1.0f, 0.0f),
        vec4(0.0f,                   0.0f,               0.0f,               1.0f)
    );
};
WorldCell::WorldCell(std::array<int, 3> new_cell_id, WorldCell* parent, int side_idx) {
    buildSides(this);
    cell_id = new_cell_id;
    // Account for the reflective transform mandated by the parent in normals
    reflection_mat = getReflectionMatrix(parent->floor_norms[side_idx]);
    vec4 norm_four;
    std::copy(parent->floor_norms, parent->floor_norms+12, floor_norms);
    for (vec3& norm : floor_norms) {
        norm_four = vec4(norm, 1.0)*reflection_mat;
        norm = vec3(norm_four.x, norm_four.y, norm_four.z);
    }
    reflection_mat *= parent->reflection_mat;
    origin = getNeighborOffset(parent->floor_norms[side_idx]) + parent->origin;
    cell_matrix = translate(cell_matrix, origin)*reflection_mat;
};
std::size_t WorldCell::generateVerts(GLfloat* buffer, std::size_t max_size) {
    std::memcpy(buffer, &ref_dodecahedron.prim_cell_verts, max_size);
    return max_size;
};
std::size_t WorldCell::generateIndxs(GLuint* buffer, std::size_t max_size) {
    uint elements_placed = 0;
    std::size_t next_size;
    std::size_t total_size = 0;
    for (int i = 0; i < 12; i++) {
        if (doors[i] == NULL) {
            next_size = sides[i].num_faces*3*sizeof(GLuint);
            if (total_size+next_size > max_size) {
                throw std::runtime_error("Not enough space, increase MAX_CELL_SIDES");
            }
            std::memcpy(buffer+elements_placed, sides[i].indices, next_size);
            elements_placed += sides[i].num_faces*3;
            total_size += next_size;
        }
    }
    return total_size;
};
std::array<int, 3> WorldCell::getNeighborID(int side_idx) {
    //TODO : need an aproach that is not deraied by side_idx order!!!
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
    //if (!isReflected(cell_id)) {
        for (int i = 0; i < 3; i++) id_array[i] = cell_id[i] + id_array[i];
    //} else {
        //for (int i = 0; i < 3; i++) id_array[i] = cell_id[i] - id_array[i];
    //}
    return id_array;
};
/* mat4 WorldCell::getNeighborMat(int side_idx) {
    mat4 new_mat;
    vec3 origin_change, normal;

    normal = floor_norms[side_idx];
    std::cout << to_string(floor_norms[0]) << std::endl;
    origin_change = getNeighborOffset(normal);
    if (isReflected(cell_id)) { 
        std::cout << "FLIP" << std::endl;
        normal *= -1.0f;
    }     

    new_mat = getReflectionMatrix(normal);
    if (!isReflected(cell_id)) {
        new_mat = translate(new_mat, origin_change+origin);
    } else {
        
    }
    return new_mat;
}; */
void WorldCell::addDoor(int door_idx, WorldCell& destination) {
    if (doors[door_idx] == NULL) {
        doors[door_idx] = &destination;
        destination.doors[door_idx] = this;
    } /* else if (doors[door_idx] != &destination) {
        throw std::runtime_error("World map overlapping?");
    } */
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
