#pragma once
#include "Shader.h"
#include "Game.h"
#include <glm/glm.hpp>

// Disegna soltanto le monete con il vertex shader animato.
// Le Geometry e le istanze restano proprieta' della Scene.
class CoinRenderer {
public:
    explicit CoinRenderer(const Shader& shader) : m_shader(shader) {}
    void draw(const Scene& scene, const Game& game,
              const glm::mat4& projection, float timeSeconds) const;
private:
    const Shader& m_shader;
};
