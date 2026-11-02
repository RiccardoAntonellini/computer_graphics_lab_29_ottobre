#pragma once
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <limits>
class Renderer {
    const Shader& m_shader;
    glm::mat4 m_projection{1.0f};
public:
    explicit Renderer(const Shader& shader):m_shader(shader){}
    void setOrthoProjection(float halfWidth,float halfHeight);
    void drawScene(const Scene& scene, std::size_t excludedGeometryIndex =
        std::numeric_limits<std::size_t>::max()) const;
    const glm::mat4& projectionMatrix() const { return m_projection; }
};
