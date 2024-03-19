#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "shape.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");
        
    ShaderProgram shader_prog("programs/cube-vertex.glsl", \
                                "programs/cube-fragment.glsl", false);

    float time;
    PlayerLocation player_location(&time);
    WorldCell world_cell = WorldCell();
    player_location.reference_cell = &world_cell;

    VAO dodecahedron_vao((GLfloat*) world_cell.cell_verts, sizeof(world_cell.cell_verts), 
                        dodecahedron_colors, sizeof(dodecahedron_colors), 
                            // TODO : pointless colors, from shape.h... (just for experimentation)
                        (GLuint*) world_cell.cell_indxs, sizeof(world_cell.cell_indxs));
    dodecahedron_vao.LinkAttrib(dodecahedron_vao.vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    dodecahedron_vao.LinkAttrib(dodecahedron_vao.cbo, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    Uniforms* uniforms  = getUniforms(window);
    
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
            uniforms->player_location = &player_location;
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

        dodecahedron_vao.DrawElements(GL_TRIANGLES, 12*9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
