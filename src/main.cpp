#include "config.h"
#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "models.h"
#include <chrono>
#include <thread>

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "DODECAPLEX");

    ShaderProgram world_shader( SHADER_DIR "/displacement.vert", \
                                SHADER_DIR "/spell.frag", false);
    ShaderProgram book_shader(  SHADER_DIR "/book.vert",\
                                SHADER_DIR "/book.frag", false);
    
    
    PlayerContext player_context;
    /* world_shader.Activate(); */
    player_context.linkPlayerCellVAOs();
    
    SpellLog spell_log;    
    spell_log.linkGrimoireVAO();

    Uniforms* uniforms = getUniforms(window);
    
    float time;
    GLuint  U_RESOLUTION, U_MOUSE, U_SCROLL, U_TIME, U_CAMERA, U_WORLD, \
            U_SPELL_LIFE, U_CAST_LIFE, U_SPELL_FOCUS, U_SPELL_HEAD;
    CameraMats mats;

    TextureLibrary texture_library;

    while (!glfwWindowShouldClose(window)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (uniforms->loading) {
            world_shader.Load();
            world_shader.Activate();

            U_RESOLUTION  = glGetUniformLocation(world_shader.ID, "u_resolution");
            U_MOUSE       = glGetUniformLocation(world_shader.ID, "u_mouse");
            U_SCROLL      = glGetUniformLocation(world_shader.ID, "u_scroll");
            U_TIME        = glGetUniformLocation(world_shader.ID, "u_time");
            U_CAMERA      = glGetUniformLocation(world_shader.ID, "CAMERA");
            U_WORLD       = glGetUniformLocation(world_shader.ID, "WORLD");
            U_CAST_LIFE   = glGetUniformLocation(world_shader.ID, "CAST_LIFE");
            U_SPELL_LIFE  = glGetUniformLocation(world_shader.ID, "SPELL_LIFE");            
            U_SPELL_FOCUS = glGetUniformLocation(world_shader.ID, "SPELL_FOCUS");
            U_SPELL_HEAD  = glGetUniformLocation(world_shader.ID, "SPELL_HEAD");

            texture_library.linkPentagonLibrary(world_shader.ID);
            
            book_shader.Load();
            book_shader.Activate();
            
            texture_library.linkGrimoireLibrary(book_shader.ID);
            
            uniforms->last_time         = glfwGetTime();
            uniforms->loading           = false;
            uniforms->player_context    = &player_context;
            
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            world_shader.Activate();
        }
        time = glfwGetTime();
        uniforms->this_time = time;
        
        /* world_shader.Activate(); */
        
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUniform2f(U_RESOLUTION,   uniforms->windWidth, \
                                    uniforms->windHeight);
        glUniform2f(U_MOUSE,        uniforms->mouseX, \
                                    uniforms->mouseY);
        glUniform1f(U_SCROLL,       uniforms->scroll);
        glUniform1f(U_TIME, time);
        
        accountSpells(uniforms, spell_log, world_shader.ID);

        glUniform1f(U_CAST_LIFE,    spell_log.cast_life[spell_log.active_spell]);
        glUniform1f(U_SPELL_LIFE,   spell_log.spell_life[spell_log.active_spell]);
        glUniform3f(U_SPELL_FOCUS,  spell_log.spell_focus[spell_log.active_spell].x,
                                    spell_log.spell_focus[spell_log.active_spell].y,
                                    spell_log.spell_focus[spell_log.active_spell].z);
        glUniform3f(U_SPELL_HEAD,   spell_log.spell_head[spell_log.active_spell].x,
                                    spell_log.spell_head[spell_log.active_spell].y,
                                    spell_log.spell_head[spell_log.active_spell].z);

        accountCameraControls(uniforms, mats);

        glUniformMatrix4fv(U_CAMERA, 1, GL_FALSE, &(mats.Projection*mats.View)[0][0]);
        glUniformMatrix4fv(U_WORLD,  1, GL_FALSE, &(mats.Model)[0][0]);
        
        player_context.drawPlayerCellVAOs(); 

        /* book_shader.Activate();  */
        spell_log.drawGrimoireVAO();

        glfwSwapBuffers(window);
        glfwPollEvents();
        uniforms->last_time = time;
    }
    return 0;
}