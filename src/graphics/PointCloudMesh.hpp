#pragma once
#include <glad/glad.h>
#include <vector>

struct Vertex {
    float x, y, z;
    float r, g, b;
};

class PointCloudMesh {
public:
    PointCloudMesh();
    ~PointCloudMesh();

    void upload(const std::vector<Vertex>& vertices);
    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLsizei m_count = 0;
};
