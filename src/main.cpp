#include "window.h"
#include "shaderClass.h"
#include "bufferObjects.h"
#include "shape.h"
#include "models.h"

int main() {
    GLFWwindow* window = initializeWindow(768, 768, "SHADER");
        
    ShaderProgram shader_prog("programs/cube-vertex.glsl", \
                                "programs/cube-fragment.glsl", false);

    std::vector<VAO> model_vaos, frame;
    for (int i=1; i < 31; i++){
        
        std::string fileName = "models/demon_a_obj/sprint_draft_2/demon_a_rigg_"+
            std::string(6 - std::to_string(i).length(), '0') + std::to_string(i)+".obj";

        frame = load_model(fileName.c_str());
        model_vaos.push_back(frame[0]);

    }

    Uniforms* uniforms  = getUniforms(window);

    float time;
    float framerate = 24.0f;
    uint current_frame = 0;
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
        
        current_frame = (static_cast<uint>(time*framerate)) % model_vaos.size();        
        model_vaos[current_frame].DrawElements(GL_TRIANGLES, 10914, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
