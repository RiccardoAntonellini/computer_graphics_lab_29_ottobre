#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
class Shader {
    GLuint m_program=0;
public:
    Shader(const std::string& vertexSource,const std::string& fragmentSource);
    ~Shader();
    Shader(const Shader&)=delete;
    Shader& operator=(const Shader&)=delete;
    void use() const;
    void setMat4(const char* uniformName,const glm::mat4& value) const;
    void setFloat(const char* uniformName, float value) const;
};
