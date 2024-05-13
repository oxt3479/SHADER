#include "config.h"
#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");

    ShaderProgram shader_prog(  SHADER_DIR "/displacement.vert", \
                                SHADER_DIR "/textured.frag", false);
    
    PlayerContext player_context;    
    player_context.linkPlayerCellVAOs();
    
    Uniforms* uniforms = getUniforms(window);
    
    float time;
    GLuint  U_RESOLUTION, U_MOUSE, U_SCROLL, U_TIME, U_CAMERA, U_WORLD, \
            U_SPELL_LIFE, U_CAST_LIFE, U_SPELL_FOCUS, U_SPELL_HEAD;
    CameraMats mats;
    SpellLog spells;

    while (!glfwWindowShouldClose(window)) {
        if (uniforms->loading) {
            shader_prog.Load();
            shader_prog.Activate();

            U_RESOLUTION  = glGetUniformLocation(shader_prog.ID, "u_resolution");
            U_MOUSE       = glGetUniformLocation(shader_prog.ID, "u_mouse");
            U_SCROLL      = glGetUniformLocation(shader_prog.ID, "u_scroll");
            U_TIME        = glGetUniformLocation(shader_prog.ID, "u_time");
            U_CAMERA      = glGetUniformLocation(shader_prog.ID, "CAMERA");
            U_WORLD       = glGetUniformLocation(shader_prog.ID, "WORLD");
            U_CAST_LIFE   = glGetUniformLocation(shader_prog.ID, "CAST_LIFE");
            U_SPELL_LIFE  = glGetUniformLocation(shader_prog.ID, "SPELL_LIFE");            
            U_SPELL_FOCUS = glGetUniformLocation(shader_prog.ID, "SPELL_FOCUS");
            U_SPELL_HEAD  = glGetUniformLocation(shader_prog.ID, "SPELL_HEAD");

            addUniformRGBATexture(shader_prog.ID, "rgbTexture", \
                TEXTURE_DIR "/tile_floor_a.png", 0);
            addUniformRGBATexture(shader_prog.ID, "depthTexture", \
                TEXTURE_DIR "/disp_test.png", 1);

            uniforms->last_time         = glfwGetTime();
            uniforms->loading           = false;
            uniforms->player_context    = &player_context;
            
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
        }
        time = glfwGetTime();
        uniforms->this_time = time;

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUniform2f(U_RESOLUTION,   uniforms->windWidth, \
                                    uniforms->windHeight);
        glUniform2f(U_MOUSE,        uniforms->mouseX, \
                                    uniforms->mouseY);
        glUniform1f(U_SCROLL,       uniforms->scroll);
        glUniform1f(U_TIME, time);
        
        accountSpells(uniforms, spells);

        glUniform1f(U_CAST_LIFE,    spells.cast_life[spells.active_spell]);
        glUniform1f(U_SPELL_LIFE,   spells.spell_life[spells.active_spell]);
        glUniform3f(U_SPELL_FOCUS,  spells.spell_focus[spells.active_spell].x,
                                    spells.spell_focus[spells.active_spell].y,
                                    spells.spell_focus[spells.active_spell].z);
        glUniform3f(U_SPELL_HEAD,   spells.spell_head[spells.active_spell].x,
                                    spells.spell_head[spells.active_spell].y,
                                    spells.spell_head[spells.active_spell].z);

        accountCameraControls(uniforms, mats);

        glUniformMatrix4fv(U_CAMERA, 1, GL_FALSE, &(mats.Projection*mats.View)[0][0]);
        glUniformMatrix4fv(U_WORLD,  1, GL_FALSE, &(mats.Model)[0][0]);
        
        player_context.drawPlayerCellVAOs();

        glfwSwapBuffers(window);
        glfwPollEvents();
        uniforms->last_time = time;
    }
    return 0;
}
