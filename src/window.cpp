#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <valarray>
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
unsigned int windWidth = 768, windHeight = 768;
float mouseX, mouseY;
float scroll = 1.0;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void resizeCallback(GLFWwindow *window, int width, int height) {
    windWidth = width;
    windHeight = height;
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = float(xpos);
    mouseY = float(ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    scroll += float(yoffset/10.0);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windWidth, windHeight, "SHADER", NULL, NULL);
    
    if (window != NULL) {

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

    ShaderProgram shaderProgram("../programs/vertex.glsl", "../programs/magic-spell.glsl");
    VAO vertArrayObj(vertices, sizeof(vertices), indices, sizeof(indices));
    vertArrayObj.LinkAttrib( 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0 );

    GLint U_RESOLUTION  = glGetUniformLocation(shaderProgram.ID, "u_resolution");
    GLint U_MOUSE       = glGetUniformLocation(shaderProgram.ID, "u_mouse");
    GLint U_TIME        = glGetUniformLocation(shaderProgram.ID, "u_time");
    GLint U_SCROLL      = glGetUniformLocation(shaderProgram.ID, "u_scroll");
    
    while (!glfwWindowShouldClose(window)) {

        shaderProgram.Activate(); 

        glClearColor(0.f, 0.f, 0.f, 1.0f); // Sets background
        glClear(GL_COLOR_BUFFER_BIT); // Clears the last buffer...

        glUniform2f(U_RESOLUTION, windWidth, windHeight);
        glUniform2f(U_MOUSE, mouseX, mouseY);
        glUniform1f(U_TIME, glfwGetTime());
        glUniform1f(U_SCROLL, scroll);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    vertArrayObj.Delete();
	glfwDestroyWindow(window); 

    } else  std::cout << "Failed to initialize GLAD"    << std::endl;   FAILURE_STATUS = -1;
    } else  std::cout << "Failed to create GLFW window" << std::endl;   FAILURE_STATUS = -1;
    
    glfwTerminate();
    return FAILURE_STATUS;
}

