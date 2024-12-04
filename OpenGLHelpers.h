#ifndef OPENGL_HELPERS_H
#define OPENGL_HELPERS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    void initialize();
    void render();
    void handleInput(GLFWwindow* window);
    void cleanup();

private:
    GLFWwindow* window;
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;

    void setupShaders();
    void setupBuffers();
    void resetCamera();
};

#endif // OPENGL_HELPERS_H
