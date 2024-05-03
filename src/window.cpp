#include "window.h"
#include "debug.h"

bool window_is_focused = false;

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

void resizeCallback(GLFWwindow* window, int width, int height) {
    Uniforms* uniforms = getUniforms(window);
    uniforms->windWidth = width;
    uniforms->windHeight = height;
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Uniforms* uniforms = getUniforms(window);
    uniforms->mouseX = float(xpos);
    uniforms->mouseY = float(ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Uniforms* uniforms = getUniforms(window);
    uniforms->scroll += float(yoffset/10.0);
}

GLFWwindow* initializeWindow(unsigned int start_width, unsigned int start_height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    Uniforms* uniforms = new Uniforms();
    uniforms->windWidth = start_width;
    uniforms->windHeight = start_height;
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

CameraMats getCameraMats(GLFWwindow* window, CameraMats &camera_mats) {
    Uniforms* uniforms = getUniforms(window);
    //PlayerLocation* player_location = uniforms->player_context->player_location;
    PlayerLocation* player_location = uniforms->player_context->player_location;
    float ratio = float(uniforms->windWidth)/float(uniforms->windHeight);
    float dt = std::min( float(uniforms->this_time-uniforms->last_time), 0.002f);
    
    // Projection matrix: 90° Field of View, display range: 0.1 unit <-> 100 units
    camera_mats.Projection  = glm::perspective(glm::radians(89.0f), ratio, 0.1f, 10.0f);
    camera_mats.View        = player_location->getView( uniforms->mouseX/float(uniforms->windWidth),
                                                        uniforms->mouseY/float(uniforms->windHeight), dt);
    camera_mats.Model       = player_location->getModel( uniforms->getWASD(), dt);
    
    return camera_mats;
}