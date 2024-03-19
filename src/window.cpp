#include "window.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Uniforms* uniforms = getUniforms(window);
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else {
        if ( key == GLFW_KEY_SPACE && action == GLFW_PRESS ) {
            uniforms->loading = true;
            std::cout << "Reloading Shaders" << std::endl;
        } else {
            uniforms->loading = false;
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

Uniforms* getUniforms(GLFWwindow* window) {
    return static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
}

glm::mat4 getCamera(GLFWwindow* window) {
    Uniforms* uniforms = getUniforms(window);
    PlayerLocation * player_location = uniforms->player_location;
    float ratio = float(uniforms->windHeight)/float(uniforms->windWidth);
    
    // Projection matrix: 90° Field of View, display range: 0.1 unit <-> 100 units
    glm::mat4 Projection    = glm::perspective(glm::radians(89.0f), ratio, 0.1f, 10.0f);
    glm::mat4 View          = player_location->getView( uniforms->mouseX/float(uniforms->windWidth),
                                                        uniforms->mouseY/float(uniforms->windHeight));
    glm::mat4 Model         = player_location->getModel();
    glm::mat4 mvp           = Projection * View * Model;
    return mvp;
}