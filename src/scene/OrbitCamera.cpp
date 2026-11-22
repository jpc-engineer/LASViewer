#include "scene/OrbitCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

void OrbitCamera::rotate(float dx, float dy) {
    m_yaw -= dx * 0.005f;
    m_pitch = std::clamp(m_pitch + dy * 0.005f, -1.55f, 1.55f);
}

void OrbitCamera::pan(float dx, float dy) {
    glm::vec3 forward(
        std::cos(m_pitch) * std::sin(m_yaw),
        std::sin(m_pitch),
        std::cos(m_pitch) * std::cos(m_yaw)
    );
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
    glm::vec3 up = glm::cross(forward, right);

    m_target -= right * dx * m_distance * 0.0012f;
    m_target += up * dy * m_distance * 0.0012f;
}

void OrbitCamera::zoom(float yOffset) {
    m_distance *= (yOffset > 0.0f) ? 0.9f : 1.1f;
    m_distance = std::clamp(m_distance, 0.5f, 100000.0f);
}

void OrbitCamera::fitToRadius(float radius) {
    m_target = glm::vec3(0.0f);
    m_distance = radius * 2.2f;
}

glm::mat4 OrbitCamera::getMVP(float aspectRatio) const {
    glm::vec3 camPos(
        m_target.x + m_distance * std::cos(m_pitch) * std::sin(m_yaw),
        m_target.y + m_distance * std::sin(m_pitch),
        m_target.z + m_distance * std::cos(m_pitch) * std::cos(m_yaw)
    );

    glm::mat4 view = glm::lookAt(camPos, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100000.0f);
    return proj * view;
}
