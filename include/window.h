#ifndef WINDOW_H
#define WINDOW_H

#include "spells.h"
#include <array>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

struct Uniforms
{
    unsigned int windWidth, windHeight;
    float mouseX, mouseY;
    float scroll = 1.0;
    bool loading = true;
    PlayerContext* player_context;
    bool key_states[1024] = {false};
    bool click_states[3] = {false};
    float click_times[3] = {NULL};
    std::array<bool, 4> getWASD();
    float last_time = 0.0f;
    float this_time = 0.0f;
};
Uniforms* getUniforms(GLFWwindow* window);

struct CameraMats
{
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;
};
void accountCameraControls(Uniforms* uniforms, CameraMats& camera_mats);

const int MAX_SPELLS = 2;
struct SpellLog
{
    // {no-spell, teleport_a}
    int active_spell = 1;
    float click_times               [MAX_SPELLS] = {};
    float release_times             [MAX_SPELLS] = {};
    const float spell_durrations    [MAX_SPELLS] = {NULL, 2.5f};
    const float cast_durrations     [MAX_SPELLS] = {NULL, 1.0f};
    float spell_life                [MAX_SPELLS] = {};
    float cast_life                 [MAX_SPELLS] = {};
    glm::vec3 spell_focus           [MAX_SPELLS] = {};
    glm::vec3 spell_head            [MAX_SPELLS] = {};
    glm::vec3 cast_intercepts       [MAX_SPELLS] = {};
    glm::vec3 cast_player_up        [MAX_SPELLS] = {};
    int intercept_indeces           [MAX_SPELLS] = {};
    WorldCell* world_cells          [MAX_SPELLS] = {};
    
    void (*updateSpellFunction[MAX_SPELLS])(float, glm::vec3, glm::vec3, glm::vec3, int, WorldCell*, PlayerContext*) = {NULL, teleportA};
    void (*startSpellFunction[MAX_SPELLS])(glm::vec3, glm::vec3, glm::vec3, int, WorldCell*, PlayerContext*) = {NULL, teleportAStart};

    void updateSpellLife(float time, PlayerContext* context) {
        spell_life[active_spell] = 1.0f-std::max(0.0f, std::min(1.0f, (time-release_times[active_spell])/spell_durrations[active_spell]));
        updateSpellFunction[active_spell](
            spell_life          [active_spell], 
            spell_head          [active_spell], 
            cast_player_up      [active_spell],
            cast_intercepts     [active_spell], 
            intercept_indeces   [active_spell], 
            world_cells         [active_spell], context);
    };
    void startSpell(float time, glm::vec3 focus, glm::vec3 head, glm::vec3 player_up, WorldCell* target_cell, glm::vec3 intercept, int intercept_idx, PlayerContext* context) {
        spell_life          [active_spell] = 1.0f;
        release_times       [active_spell] = time;
        spell_focus         [active_spell] = focus;
        spell_head          [active_spell] = head;
        cast_player_up      [active_spell] = player_up;
        world_cells         [active_spell] = target_cell;
        cast_intercepts     [active_spell] = intercept;
        intercept_indeces   [active_spell] = intercept_idx;
        click_times         [active_spell] = NULL;
        startSpellFunction[active_spell](
            spell_head          [active_spell], 
            cast_player_up      [active_spell],
            cast_intercepts     [active_spell], 
            intercept_indeces   [active_spell], 
            world_cells         [active_spell], context);

    };
    void chargeSpell(float time, glm::vec3 focus, glm::vec3 head) {
        cast_life   [active_spell] = std::max(0.0f, std::min(1.0f, (time-click_times[active_spell])/cast_durrations[active_spell]));
        spell_focus [active_spell] = focus;
        spell_head  [active_spell] = head;
    };
};
void accountSpells(Uniforms* uniforms, SpellLog &spell_log);

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height, const char* title);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif