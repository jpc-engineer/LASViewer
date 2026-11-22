#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setFloat(const std::string& name, float value) const;

private:
    GLuint m_programID = 0;
    std::string readFile(const std::string& path) const;
    GLuint compileShader(GLenum type, const std::string& source) const;
};
