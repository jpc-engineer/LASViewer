#pragma once
#include <glm/glm.hpp>

class OrbitCamera {
public:
    void rotate(float dx, float dy);
    void pan(float dx, float dy);
    void zoom(float yOffset);
    void fitToRadius(float radius);

    glm::mat4 getMVP(float aspectRatio) const;

private:
    glm::vec3 m_target = glm::vec3(0.0f);
    float m_distance = 30.0f;
    float m_yaw = 0.6f;
    float m_pitch = 0.5f;
};
