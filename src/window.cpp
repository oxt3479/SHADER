#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int START_WIDTH = 768, START_HEIGHT = 512;
int FAILURE_STATUS = 0;

void processInputs(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void onWindowResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "SHADER", NULL, NULL);
    
    if (window != NULL) {

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onWindowResize);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

    while (!glfwWindowShouldClose(window)) {
        processInputs(window);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    } else  std::cout << "Failed to initialize GLAD"    << std::endl;   FAILURE_STATUS = -1;
    } else  std::cout << "Failed to create GLFW window" << std::endl;   FAILURE_STATUS = -1;
    
    glfwTerminate();
    return FAILURE_STATUS;
}

