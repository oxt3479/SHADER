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
    /*bool moving = (action == GLFW_REPEAT || action == GLFW_PRESS);
    switch (key) {
        case GLFW_KEY_W:
            uniforms->delta_z += moving ? 1.0f:0.0f;
        case GLFW_KEY_A:
            uniforms->delta_x += moving ? 1.0f:0.0f;
        case GLFW_KEY_S:
            uniforms->delta_z += moving ? -1.0f:0.0f;
        case GLFW_KEY_D:
            uniforms->delta_x += moving ? -1.0f:0.0f;
        default:
            break;
    }*/
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

    return window;
}

Uniforms* getUniforms(GLFWwindow* window) {
    return static_cast<Uniforms*>(glfwGetWindowUserPointer(window));
}

glm::mat4 getCamera(float ratio, float x, float y, float z) {
    // Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(x,y,z), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection: multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    return mvp;
}