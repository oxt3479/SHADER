#ifndef WINDOW_H
#define WINDOW_H

#include "world.h"
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
    std::array<bool, 4> getWASD();
};
Uniforms* getUniforms(GLFWwindow* window);

struct CameraMats
{
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;
};
CameraMats getCameraMats(GLFWwindow* window);

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height,\
    const char* title);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif