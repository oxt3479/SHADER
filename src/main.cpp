#include "window.h"
#include "shaderClass.h"
#include "shaderObjects.h"

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

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");

    ShaderProgram shaderProgram("../programs/vertex.glsl", \
                                "../programs/magic-spell.glsl", false);
    VAO vertArrayObj(vertices, sizeof(vertices), indices, sizeof(indices));
    vertArrayObj.LinkAttrib( 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0 );
    
    Uniforms* uniforms  = getUniforms(window);
    GLint U_RESOLUTION, U_MOUSE, U_SCROLL, U_TIME;
    
    while (!glfwWindowShouldClose(window)) {
        if (uniforms->loading) {
            shaderProgram.Load();
            shaderProgram.Activate();

            U_RESOLUTION  = glGetUniformLocation(shaderProgram.ID, "u_resolution");
            U_MOUSE       = glGetUniformLocation(shaderProgram.ID, "u_mouse");    
            U_SCROLL      = glGetUniformLocation(shaderProgram.ID, "u_scroll");
            U_TIME        = glGetUniformLocation(shaderProgram.ID, "u_time");
        }

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2f(U_RESOLUTION, uniforms->windWidth, uniforms->windHeight);
        glUniform2f(U_MOUSE, uniforms->mouseX, uniforms->mouseY);        
        glUniform1f(U_SCROLL, uniforms->scroll);
        glUniform1f(U_TIME, glfwGetTime());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
