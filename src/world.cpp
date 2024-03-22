#include "world.h"

using namespace glm;

Dodecahedron dodecahedron;

PlayerContext::PlayerContext() {

};

VAO cellToVAO(WorldCell cell) {
    VAO cell_vao(
        (GLfloat*) dodecahedron.prim_cell_verts, sizeof(dodecahedron.prim_cell_verts),
        (GLfloat*) &cell.cell_matrix, sizeof(glm::mat4),
        (GLuint*) dodecahedron.prim_cell_indxs, sizeof(dodecahedron.prim_cell_indxs)
    );
    cell_vao.LinkAttrib(cell_vao.vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    cell_vao.LinkMat4(cell_vao.cbo, 1);
    return cell_vao;
};

void PlayerContext::linkPlayerCellVAOs() {
    WorldCell first_cell = *player_location->reference_cell;
    all_vaos.push_back(cellToVAO(first_cell));
    std::array<int, 3> new_id;
    for (int i = 0; i < 12; i++) {
        new_id = first_cell.getNeighborID(i);
        
        // Check if this neighbor exists; if so don't make a new one.
        WorldCell* new_cell = world_map.cellFromID(new_id);
        if (new_cell == NULL) {
            new_cell = new WorldCell(new_id, first_cell.getNeighborMat(i));
            world_map.addCell(*new_cell);
        }

        all_vaos.push_back(cellToVAO(*new_cell));
    }
};

void PlayerContext::drawPlayerCellVAOs() {
    for (int i = 0; i < all_vaos.size(); i++){
        all_vaos[i].DrawElements(GL_TRIANGLES, 12*9, GL_UNSIGNED_INT, 0);
    }
};

WorldMap::WorldMap() {
};

int idToIdx(std::array<int, 3> cell_id) {
    int x = MAX_WORLD_SIZE/2 + cell_id[0];
    int y = MAX_WORLD_SIZE/2 + cell_id[1];
    int z = MAX_WORLD_SIZE/2 + cell_id[2];
    int index = x*MAX_WORLD_SIZE*MAX_WORLD_SIZE + y*MAX_WORLD_SIZE + z;
    if (index >= MAX_WORLD_SIZE*MAX_WORLD_SIZE*MAX_WORLD_SIZE) {
        throw std::runtime_error("Index outside of world limits");
    }
    return index;
};

WorldCell* WorldMap::cellFromID(std::array<int, 3> cell_id) {
    int index = idToIdx(cell_id);
    return cell_grid[index];
};

void WorldMap::addCell(WorldCell& world_cell) {
    int index = idToIdx(world_cell.cell_id);
    cell_grid[index] = &world_cell;
};


PlayerLocation::PlayerLocation() {
    floor_indx = getFloorIndex(); 
        // Technically, you can choose any int 0-12
    const vec3* up          = &reference_cell->floor_norms[floor_indx];
    CellSide* floor_mesh    = &reference_cell->sides[floor_indx];
    vec3 floor              = floor_mesh->findIntercept(-*up);

    feet = floor;
    head = floor + (*up)*height;
    
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
mat4 PlayerLocation::getModel(std::array<bool, 4> WASD) {
    vec3 left_right = normalize(cross(focus, feet));
    vec3 front_back = normalize(cross(left_right, feet));
    if (WASD[0]) Model = translate(Model, front_back*movement_scale);
    if (WASD[1]) Model = translate(Model, -left_right*movement_scale);
    if (WASD[2]) Model = translate(Model, -front_back*movement_scale);
    if (WASD[3]) Model = translate(Model, left_right*movement_scale);

    return Model;
};
uint PlayerLocation::getFloorIndex() {
    /*Using the players feet this gives the closest aligned
    surface normal in the dodecahedron defining the cell
    feet is the displacement from the head*/
    float min = 0.0;
    int min_idx = 0;
    for (unsigned i; i < 12; i++) {
        float product = dot(reference_cell->floor_norms[i], -feet);
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

    focus = normalize(normalize(focus) + (dx*horizontal) + (dy*vertical));

    mx = x;
    my = y;
};
/*TODO: Use more modern "track ball" approach? 
(Current implementation behaves weird lookging up or down too far)
https://github.com/RockefellerA/Trackball/blob/master/Trackball.cpp*/