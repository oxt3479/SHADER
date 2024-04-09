#include "config.h"
#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");

    ShaderProgram shader_prog(  SHADER_DIR "/stereographic.vert", \
                                SHADER_DIR "/textured.frag", false);
    
    PlayerContext player_context;    
    player_context.linkPlayerCellVAOs();
    
    Uniforms* uniforms  = getUniforms(window);
    
    float time;    
    GLuint U_RESOLUTION, U_MOUSE, U_SCROLL, U_TIME, U_CAMERA, U_WORLD;
    CameraMats mats;

    while (!glfwWindowShouldClose(window)) {
        if (uniforms->loading) {
            shader_prog.Load();
            shader_prog.addRGBTexture("textureA", TEXTURE_DIR "/tile_floor_a.png", 0);
            shader_prog.Activate();

            U_RESOLUTION  = glGetUniformLocation(shader_prog.ID, "u_resolution");
            U_MOUSE       = glGetUniformLocation(shader_prog.ID, "u_mouse");
            U_SCROLL      = glGetUniformLocation(shader_prog.ID, "u_scroll");
            U_TIME        = glGetUniformLocation(shader_prog.ID, "u_time");
            U_CAMERA      = glGetUniformLocation(shader_prog.ID, "CAMERA");
            U_WORLD       = glGetUniformLocation(shader_prog.ID, "WORLD");
    
            uniforms->loading = false;
            uniforms->player_context = &player_context;
        }

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        time = glfwGetTime();
        mats = getCameraMats(window);

        glUniform2f(U_RESOLUTION, uniforms->windWidth, uniforms->windHeight);
        glUniform2f(U_MOUSE, uniforms->mouseX, uniforms->mouseY);        
        glUniform1f(U_SCROLL, uniforms->scroll);        
        glUniform1f(U_TIME, time);
        
        glUniformMatrix4fv(U_CAMERA, 1, GL_FALSE, \
            &(mats.Projection*mats.View)[0][0]);
        glUniformMatrix4fv(U_WORLD, 1, GL_FALSE, \
            &mats.Model[0][0]);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        player_context.drawPlayerCellVAOs();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
