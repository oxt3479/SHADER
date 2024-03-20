#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");
        
    ShaderProgram shader_prog("programs/minimum-vertex.glsl", \
                                "programs/cube-fragment.glsl", false);

    PlayerContext player_context;
    player_context.linkPlayerCellVAOs();

    Uniforms* uniforms  = getUniforms(window);
    
    float time;    
    GLint U_RESOLUTION, U_MOUSE, U_SCROLL, U_TIME, U_CAMERA;

    while (!glfwWindowShouldClose(window)) {
        if (uniforms->loading) {
            shader_prog.Load();
            shader_prog.Activate();

            U_RESOLUTION  = glGetUniformLocation(shader_prog.ID, "u_resolution");
            U_MOUSE       = glGetUniformLocation(shader_prog.ID, "u_mouse");
            U_SCROLL      = glGetUniformLocation(shader_prog.ID, "u_scroll");
            U_TIME        = glGetUniformLocation(shader_prog.ID, "u_time");
            U_CAMERA      = glGetUniformLocation(shader_prog.ID, "MVP");

            uniforms->loading = false;
            uniforms->player_context = &player_context;
        }

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        time = glfwGetTime();

        glUniform2f(U_RESOLUTION, uniforms->windWidth, uniforms->windHeight);
        glUniform2f(U_MOUSE, uniforms->mouseX, uniforms->mouseY);        
        glUniform1f(U_SCROLL, uniforms->scroll);        
        glUniform1f(U_TIME, time);
        glUniformMatrix4fv(U_CAMERA, 1, GL_FALSE, \
            &getCamera(window)[0][0]);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        player_context.drawPlayerCellVAOs();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
