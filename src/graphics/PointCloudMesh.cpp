#include "graphics/PointCloudMesh.hpp"

PointCloudMesh::PointCloudMesh() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

PointCloudMesh::~PointCloudMesh() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

void PointCloudMesh::upload(const std::vector<Vertex>& vertices) {
    m_count = static_cast<GLsizei>(vertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void PointCloudMesh::draw() const {
    if (m_count == 0) return;
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_count);
}
