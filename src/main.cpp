#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "las_loader.hpp"

// Embedded GLSL Shaders 
const char* vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;
uniform mat4 uMVP;
uniform float uPointSize;

void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = uPointSize;
    vColor = aColor;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

struct CameraState {
    glm::vec3 target = glm::vec3(0.0f);
    float distance = 30.0f;
    float yaw = 0.6f;
    float pitch = 0.5f;
    bool rotating = false;
    bool panning = false;
    double lastX = 0.0, lastY = 0.0;
} cam;

GLuint VBO = 0;
size_t pointcount = 0;

void uploadVertices(const std::vector<Vertex>& verts) {
    pointcount = verts.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
}

// Generate a default mathematical 3D wave cloud on startup
std::vector<Vertex> makeDemoCloud() {
    std::vector<Vertex> verts;
    for (float x = -10.0f; x <= 10.0f; x += 0.025f) {
        for (float z = -10.0f; z <= 10.0f; z += 0.25f) {
            float y = std::sin(x * 0.5f) * std::cos(z * 0.5f) * 2.5f;
            float r, g, b;
            elevationToRGB((y + 2.5f) / 5.0f, r, g, b);
            verts.push_back({x, y, z, r, g, b});
        }
    }
    return verts;
}

int main() {

    // Required for MacOS OpenGL core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "LAS Point Cloud Viewer", nullptr, nullptr);

    if (!window) return -1;

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Compile shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fs);

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Setup VAO and VBO

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
