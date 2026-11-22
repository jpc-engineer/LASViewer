#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

class Window {
public:
    using DragCallback = std::function<void(float dx, float dy, bool isRightClick)>;
    using ScrollCallback = std::function<void(float yOffset)>;
    using DropCallback = std::function<void(const std::string& filepath)>;

    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void beginFrame(float r, float g, float b);
    void endFrame();
    float getAspectRatio() const;

    void onMouseDrag(DragCallback cb) { m_dragCb = std::move(cb); }
    void onScroll(ScrollCallback cb) { m_scrollCb = std::move(cb); }
    void onFileDrop(DropCallback cb) { m_dropCb = std::move(cb); }

private:
    GLFWwindow* m_window = nullptr;
    bool m_leftPressed = false;
    bool m_rightPressed = false;
    double m_lastX = 0.0;
    double m_lastY = 0.0;

    DragCallback m_dragCb;
    ScrollCallback m_scrollCb;
    DropCallback m_dropCb;
};
