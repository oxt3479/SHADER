#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <valarray>
#include <chrono>
#include "shaderClass.h"
#include "shaderObjects.h"
//-----------------------
GLfloat vertices[] =
{
    -1.0f,  1.0f, // Top-left
    -1.0f, -1.0f, // Bottom-left
     1.0f, -1.0f, // Bottom-right
     1.0f,  1.0f  // Top-right
};

GLuint indices[] =
{
    0, 1, 2, // First Triangle
    2, 3, 0  // Second Triangle
};
//-----------------------

int FAILURE_STATUS = 0;

void processInputs(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int WIDTH = 768, HEIGHT = 768;
void resizeCallback(GLFWwindow *window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

float mouseX = 0.0, mouseY = 0.0;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

const auto startTime = std::chrono::high_resolution_clock::now();
float getTime() {
    return std::chrono::duration<float, std::chrono::seconds::period>
            (std::chrono::high_resolution_clock::now() - startTime).count();
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "SHADER", NULL, NULL);
    
    if (window != NULL) {

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

    ShaderProgram shaderProgram("../programs/vertex.glsl", "../programs/purple-vortex.glsl");
    VAO VAO1(vertices, sizeof(vertices), indices, sizeof(indices));
    VAO1.LinkAttrib( 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0 );

    GLint U_RESOLUTION  = glGetUniformLocation(shaderProgram.ID, "u_resolution");
    GLint U_MOUSE       = glGetUniformLocation(shaderProgram.ID, "u_mouse");
    GLint U_TIME        = glGetUniformLocation(shaderProgram.ID, "u_time");
    GLint U_SCROLL      = glGetUniformLocation(shaderProgram.ID, "u_scroll");
    
    while (!glfwWindowShouldClose(window)) {
        
        processInputs(window);
        shaderProgram.Activate(); 

        glClearColor(0.f, 0.f, 0.f, 1.0f); // Sets background
        glClear(GL_COLOR_BUFFER_BIT); // Clears the last buffer...

        glUniform2f(U_RESOLUTION, float(WIDTH), float(HEIGHT));
        glUniform2f(U_MOUSE, mouseX, mouseY);
        glUniform1f(U_TIME, getTime());
        glUniform1f(U_SCROLL, 1.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    VAO1.Delete();
	glfwDestroyWindow(window); 

    } else  std::cout << "Failed to initialize GLAD"    << std::endl;   FAILURE_STATUS = -1;
    } else  std::cout << "Failed to create GLFW window" << std::endl;   FAILURE_STATUS = -1;
    
    glfwTerminate();
    return FAILURE_STATUS;
}

