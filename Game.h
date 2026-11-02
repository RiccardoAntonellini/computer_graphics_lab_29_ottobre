#pragma once
#include "Scene.h"
#include <glm/glm.hpp>
#include <cstddef>
#include <vector>

namespace GameConfig {
constexpr float HALF_HEIGHT = 60.0f;
constexpr float FLOOR_Y = -48.0f;
constexpr float PLAYER_WIDTH = 7.0f;
constexpr float PLAYER_HEIGHT = 11.0f;
constexpr float MOVE_SPEED = 43.0f;
constexpr float JUMP_SPEED = 75.0f; // salto sufficiente per la prima piattaforma
constexpr float GRAVITY = -135.0f;
constexpr float COIN_RADIUS = 3.0f;
constexpr float COIN_BOB_AMPLITUDE = 2.0f; // unita del mondo
constexpr float COIN_BOB_FREQUENCY = 3.0f; // radianti al secondo
constexpr float COIN_PHASE_STEP = 0.8f;
}

// Dati della simulazione, separati dalle GeometryInstance.
struct AABB { glm::vec2 center; glm::vec2 halfSize; };
struct Player {
    std::size_t instanceIndex = 0;
    glm::vec2 position{0.0f};
    glm::vec2 velocity{0.0f};
    bool onGround = true;
};
struct Obstacle {
    std::size_t instanceIndex = 0;
    glm::vec2 position{0.0f};
    glm::vec2 size{0.0f};
};
struct Collectible {
    std::size_t instanceIndex = 0;
    glm::vec2 position{0.0f};
    bool collected = false;
};

class Game {
public:
    explicit Game(Scene& scene);
    void update(float dt, float halfWidth, bool left, bool right, bool jumpPressed, float currentTime);
    void reset();
    void resize(float halfWidth);
    int score() const { return m_score; }
    std::size_t coinGeometryIndex() const { return m_coinGeometryIndex; }
    const std::vector<Collectible>& coins() const { return m_coins; }

private:
    Scene& m_scene; // Game non possiede Scene; la usa soltanto.
    Player m_player;
    std::vector<Obstacle> m_obstacles;
    std::vector<Collectible> m_coins;
    std::size_t m_floorInstance = 0;
    int m_score = 0;
    std::size_t m_coinGeometryIndex = 0;
    float m_currentTime = 0.0f;

    void createPlayer(std::size_t playerGeo);
    void createFloor(std::size_t floorGeo);
    void createObstacles(std::size_t obstacleGeo);
    void createCoins(std::size_t coinGeo);
    void updatePlayer(float dt, float halfWidth, bool left, bool right, bool jumpPressed);
    void collectCoins();
    void updateModelMatrices();
};
