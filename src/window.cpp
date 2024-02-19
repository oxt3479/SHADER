#include "window.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    Uniforms* uniforms = static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
    uniforms->windWidth = width;
    uniforms->windHeight = height;
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Uniforms* uniforms = static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
    uniforms->mouseX = float(xpos);
    uniforms->mouseY = float(ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Uniforms* uniforms = static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
    uniforms->scroll += float(yoffset/10.0);
}

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(start_width, start_height, title, NULL, NULL);
    
    if (window == NULL) { 
        std::cout << "Failed to create GLFW window" << std::endl; 
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    Uniforms* uniforms = new Uniforms();
    uniforms->windWidth = start_width;
    uniforms->windHeight = start_height;
    glfwSetWindowUserPointer(window, uniforms);

    return window;
}