#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "shape.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");
        
    ShaderProgram shader_prog("../programs/cube-vertex.glsl", \
                                "../programs/cube-fragment.glsl", false);

    std::vector<VAO> model_vaos = load_model("../models/demon_a_obj/model.obj");

    for (int i=0; i < (int) model_vaos.size(); i++) {
        model_vaos[i].LinkAttrib(model_vaos[i].vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        model_vaos[i].LinkAttrib(model_vaos[i].vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        model_vaos[i].LinkAttrib(model_vaos[i].vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

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
        }

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        time = glfwGetTime();

        glUniform2f(U_RESOLUTION, uniforms->windWidth, uniforms->windHeight);
        glUniform2f(U_MOUSE, uniforms->mouseX, uniforms->mouseY);        
        glUniform1f(U_SCROLL, uniforms->scroll);        
        glUniform1f(U_TIME, time);
        glUniformMatrix4fv(U_CAMERA, 1, GL_FALSE, \
            &getCamera(1.0f, 3.0*cos(time), 3.0, 3.0*sin(time))[0][0]);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        for (int i=0; i < (int) model_vaos.size(); i++) {
            model_vaos[i].DrawElements(GL_TRIANGLES, 10914, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
