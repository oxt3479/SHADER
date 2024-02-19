#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct Uniforms
{
    unsigned int windWidth, windHeight;
    float mouseX, mouseY;
    float scroll = 1.0;
    bool loading = true;
};
Uniforms* getUniforms(GLFWwindow* window);

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height,\
    const char* title);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

