#include "CoinRenderer.h"
#include <glad/glad.h>

void CoinRenderer::draw(const Scene& scene, const Game& game,
                        const glm::mat4& projection, float timeSeconds) const
{
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setFloat("uTime", timeSeconds);
    m_shader.setFloat("uAmplitude", GameConfig::COIN_BOB_AMPLITUDE);
    m_shader.setFloat("uFrequency", GameConfig::COIN_BOB_FREQUENCY);

    const auto& coins = game.coins();
    for (std::size_t i = 0; i < coins.size(); ++i) {
        const Collectible& coin = coins[i];
        const GeometryInstance& instance = scene.instances().at(coin.instanceIndex);
        if (coin.collected || !instance.visible) continue;
        // Ogni moneta ha fase distinta, pur condividendo la stessa mesh.
        m_shader.setFloat("uPhase", static_cast<float>(i) * GameConfig::COIN_PHASE_STEP);
        m_shader.setMat4("model", instance.modelMatrix);
        scene.geometry(instance.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
