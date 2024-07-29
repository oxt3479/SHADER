#include "window.h"
#include "debug.h"
#include <deque>
#include <iostream>
#include <typeinfo>

bool window_is_focused = false;

std::deque<std::pair<double, double>> mouse_positions;
const size_t max_positions = 10; // Adjust this value for more/less smoothing

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Uniforms* uniforms = getUniforms(window);

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        } else if (key == GLFW_KEY_SPACE) {
            uniforms->loading = true;
            std::cout << "Reloading Shaders" << std::endl;            
        } else if (key == GLFW_KEY_TAB) {
            if (window_is_focused) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                window_is_focused = false;
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                window_is_focused = true;
            }
        }
        uniforms->key_states[key] = true;
    } else if (action == GLFW_RELEASE) {
        uniforms->key_states[key] = false;
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Uniforms* uniforms = getUniforms(window);
    if (button < 3) { // AKA, left. right, middle
        if (action == GLFW_PRESS) {
            uniforms->click_states[button] = true;
            uniforms->click_times[button] = glfwGetTime();
        } else if (action == GLFW_RELEASE) {
            uniforms->click_states[button] = false;
        }
    }
}

void resizeCallback(GLFWwindow* window, int width, int height) {
    Uniforms* uniforms = getUniforms(window);
    uniforms->windWidth = width;
    uniforms->windHeight = height;
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Uniforms* uniforms = getUniforms(window);

    uniforms->mouseX = float(xpos) / float(uniforms->windWidth);
    uniforms->mouseY = float(ypos) / float(uniforms->windHeight);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Uniforms* uniforms = getUniforms(window);
    uniforms->scroll += float(yoffset/10.0);
}

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(start_width, start_height, title, NULL, NULL);
    
    if (window == NULL) { 
        std::cout << "Failed to create GLFW window" << std::endl; 
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, start_width, start_height);
    
    int system_width, system_height;
    glfwGetFramebufferSize(window, &system_width,
                                   &system_height);
    glViewport(0, 0, system_width, system_height);
    // Resize necessary on OSX, due to scaling nonsense...

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    glfwSetKeyCallback(             window, keyCallback);
    glfwSetScrollCallback(          window, scrollCallback);
    glfwSetCursorPosCallback(       window, mouseCallback);
    glfwSetMouseButtonCallback(     window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback( window, resizeCallback);    
    glfwSetInputMode(               window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); // Enable raw mouse motion

    Uniforms* uniforms = new Uniforms();
    uniforms->windWidth = system_width;
    uniforms->windHeight = system_height;
    glfwSetWindowUserPointer(window, uniforms);
    if (window_is_focused)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    printGPUCapabilities();
    return window;
}

Uniforms* getUniforms(GLFWwindow* window) {
    return static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
}

std::array<bool, 4> Uniforms::getWASD() { 
    return {key_states[GLFW_KEY_W], key_states[GLFW_KEY_A], 
            key_states[GLFW_KEY_S], key_states[GLFW_KEY_D]};
}

void accountCameraControls(Uniforms* uniforms, CameraMats &camera_mats) {
    PlayerLocation* player_location = uniforms->player_context->player_location;
    float ratio = float(uniforms->windWidth)/float(uniforms->windHeight);
    float dt = std::min( float(uniforms->this_time-uniforms->last_time), 0.01f);
    
    // Projection matrix: 90° Field of View, display range: 0.1 unit <-> 100 units
    camera_mats.Projection  = glm::perspective(glm::radians(89.0f), ratio, 0.1f, 10.0f);
    camera_mats.View        = player_location->getView( uniforms->mouseX*5.0f,
                                                        uniforms->mouseY*5.0f, dt);
    camera_mats.Model       = player_location->getModel( uniforms->getWASD(), dt);
}

void accountSpells(Uniforms* uniforms, SpellLog &spell_log) {
    static GLuint spell_idxs[] = {0};
    static glm::vec3 intercept_point;
    static int intercept_index;
    float current_time = glfwGetTime();
    if (uniforms->click_states[0] && !spell_log.spell_life[spell_log.active_spell]) {
        // The mouse is being held down... AND the spell is not currently running.
        if(!spell_log.click_times[spell_log.active_spell]) {
            // And it's the first frame of it being held down...
            spell_idxs[0] = 1;
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, spell_idxs);
            spell_log.click_times[spell_log.active_spell] = current_time;
        }
        spell_log.chargeSpell(current_time, 
                uniforms->player_context->player_location->getFocus(),
                uniforms->player_context->player_location->getHead());
    } else if (spell_log.click_times[spell_log.active_spell]) {
        // The mouse was JUST released
        spell_idxs[0] = 0;
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, spell_idxs);
        spell_log.startSpell(current_time, 
                uniforms->player_context->player_location->getFocus(),
                uniforms->player_context->player_location->getHead(),
                uniforms->player_context->player_location->getPUp(),
                uniforms->player_context->player_location->getIntercept(),
                uniforms->player_context);
    } else if (spell_log.spell_life[spell_log.active_spell]) {
        // The spell has been cast, and will decay from 1.0f to 0.0f
        // If its at 0.0f this will not be triggered..
        spell_log.updateSpellLife(current_time, uniforms->player_context);
        if (spell_log.spell_life[spell_log.active_spell] == 0.0f) {
            // The spell is complete, we change the subroutine for the last pass..
            spell_idxs[0] = 2;
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, spell_idxs);
        }
    }
}