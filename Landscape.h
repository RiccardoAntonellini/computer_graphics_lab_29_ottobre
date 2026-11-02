#pragma once
#include "Scene.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <vector>

// Fondale decorativo indipendente dalla fisica del gioco.
// Le montagne sono mesh triangolate; gli alberi condividono
// una geometria per il tronco e una per la chioma.
class Landscape {
public:
    Landscape();
    // Disegna il paesaggio prima del mondo di gioco.
    void draw(const Renderer& renderer) const;
    void clear();
private:
    Scene m_scene;
    // Genera una montagna sinusoidale con GL_TRIANGLE_STRIP.
    // Restituisce i campioni del profilo locale, usati anche per gli alberi.
    std::vector<glm::vec2> addMountain(
        float width, float height, float amplitude,
        float frequency, float phase, int segments,
        const glm::vec2& position, const glm::vec3& color);
    static float heightOnSlope(const std::vector<glm::vec2>& profile, float x);
    void addTree(float x, float groundY, float size,
                 std::size_t trunkGeometry, std::size_t crownGeometry);
};
