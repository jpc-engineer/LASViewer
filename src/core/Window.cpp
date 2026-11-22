#include "core/Window.hpp"
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const std::string& title) {
    glfwSetErrorCallback([](int code, const char* desc) {
        std::cerr << "GLFW Error (" << code << "): " << desc << "\n";
    });

    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Store 'this' inside the GLFW window so static C callbacks can access our class
    glfwSetWindowUserPointer(m_window, this);

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (button == GLFW_MOUSE_BUTTON_LEFT) self->m_leftPressed = (action == GLFW_PRESS);
        if (button == GLFW_MOUSE_BUTTON_RIGHT) self->m_rightPressed = (action == GLFW_PRESS);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        float dx = static_cast<float>(x - self->m_lastX);
        float dy = static_cast<float>(y - self->m_lastY);
        self->m_lastX = x;
        self->m_lastY = y;

        if (self->m_dragCb && (self->m_leftPressed || self->m_rightPressed)) {
            self->m_dragCb(dx, dy, self->m_rightPressed);
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* w, double xoff, double yoff) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (self->m_scrollCb) self->m_scrollCb(static_cast<float>(yoff));
    });

    glfwSetDropCallback(m_window, [](GLFWwindow* w, int count, const char** paths) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (count > 0 && self->m_dropCb) {
            self->m_dropCb(paths[0]);
        }
    });
}

Window::~Window() {
    if (m_window) glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::beginFrame(float r, float g, float b) {
    int fbW, fbH;
    glfwGetFramebufferSize(m_window, &fbW, &fbH);
    glViewport(0, 0, fbW, fbH);

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::endFrame() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

float Window::getAspectRatio() const {
    int fbW, fbH;
    glfwGetFramebufferSize(m_window, &fbW, &fbH);
    return (fbH > 0) ? static_cast<float>(fbW) / static_cast<float>(fbH) : 1.0f;
}
