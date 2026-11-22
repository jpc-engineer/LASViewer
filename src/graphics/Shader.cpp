#include "graphics/Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    GLuint vs = compileShader(GL_VERTEX_SHADER, readFile(vertPath));
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, readFile(fragPath));

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vs);
    glAttachShader(m_programID, fs);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader link error: ") + log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    if (m_programID) glDeleteProgram(m_programID);
}

void Shader::use() const {
    glUseProgram(m_programID);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint loc = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setFloat(const std::string& name, float value) const {
    GLint loc = glGetUniformLocation(m_programID, name.c_str());
    glUniform1f(loc, value);
}

std::string Shader::readFile(const std::string& path) const {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot open shader file: " + path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& source) const {
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        throw std::runtime_error(std::string("Shader compile error: ") + log);
    }
    return id;
}
