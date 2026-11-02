#include "Game.h"
#include "ShapeGenerator.h"
#include "DecorativeShapes.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <array>
#include <cmath>

using namespace GameConfig;

namespace {
// T * S: i vertici locali rimangono nei VBO, cambia la Model Matrix.
glm::mat4 makeModel(const glm::vec2& pos, float sx = 1.0f, float sy = 1.0f)
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
           glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, 1.0f));
}

// Due rettangoli AABB si intersecano solo se si sovrappongono su X e Y.
bool intersects(const AABB& a, const AABB& b)
{
    // TODO 3 - COLLISION DETECTION (circa 50 minuti)
    // Due AABB si intersecano quando si sovrappongono SIA su X SIA su Y.
    // Confrontare la distanza tra i centri con la somma delle semidimensioni.
    // Questa versione provvisoria restituisce sempre false: il programma
    // parte, ma le collisioni con ostacoli e cristalli non sono attive.
    (void)a;
    (void)b;
    return false; // TODO 3: sostituire con il test AABB

}
} // namespace anonimo: funzioni di supporto usate solo in Game.cpp

Game::Game(Scene& scene) : m_scene(scene)
{
    // Le quattro Geometry sono create una volta sola.
    const auto playerGeo = m_scene.addGeometry(
        DecorativeShapes::robot(PLAYER_WIDTH, PLAYER_HEIGHT));
    const auto floorGeo = m_scene.addGeometry(
        ShapeGenerator::rectangle(1.0f, 1.0f, {0.25f, 0.62f, 0.38f}));
    const auto obstacleGeo = m_scene.addGeometry(
        DecorativeShapes::platformRock());
    const auto coinGeo = m_scene.addGeometry(
        DecorativeShapes::crystal(COIN_RADIUS));

    createPlayer(playerGeo);
    createFloor(floorGeo);
    createObstacles(obstacleGeo);
    m_coinGeometryIndex = coinGeo;
    createCoins(coinGeo);
    updateModelMatrices();
}

void Game::createPlayer(std::size_t playerGeo)
{
    m_player.instanceIndex = m_scene.addInstance(playerGeo);
    m_player.position = {-92.0f, FLOOR_Y + PLAYER_HEIGHT * 0.5f};
    m_player.velocity = {0.0f, 0.0f};
    m_player.onGround = true;
}

void Game::createFloor(std::size_t floorGeo)
{
    m_floorInstance = m_scene.addInstance(floorGeo);
}

void Game::createObstacles(std::size_t obstacleGeo)
{
    // Ogni elemento: {centro X, larghezza, altezza}.
    const std::vector<std::array<float, 3>> specs = {
        {{-43.0f, 10.0f, 15.0f}},
        {{  0.0f, 12.0f, 22.0f}},
        {{ 48.0f, 10.0f, 18.0f}}
    };
    const auto grassGeo = m_scene.addGeometry(DecorativeShapes::grassStrip());
    for (const auto& spec : specs) {
        Obstacle o;
        o.instanceIndex = m_scene.addInstance(obstacleGeo);
        o.position = {spec[0], FLOOR_Y + spec[2] * 0.5f};
        o.size = {spec[1], spec[2]};
        m_scene.instance(o.instanceIndex).modelMatrix =
            makeModel(o.position, o.size.x, o.size.y);
        // Il prato e' decorativo: non modifica la AABB dell'ostacolo.
        const std::size_t grass = m_scene.addInstance(grassGeo);
        m_scene.instance(grass).modelMatrix =
            makeModel({o.position.x, FLOOR_Y + o.size.y - 0.45f}, o.size.x, 1.3f);
        m_obstacles.push_back(o);
    }
}

void Game::createCoins(std::size_t coinGeo)
{
    const std::vector<glm::vec2> positions = {
        {-68.0f, -27.0f}, {-43.0f, -21.0f}, {-18.0f, -23.0f},
        {0.0f, -13.0f}, {24.0f, -28.0f}, {48.0f, -17.0f}, {77.0f, -28.0f}
    };
    for (const glm::vec2& pos : positions) {
        Collectible coin;
        coin.instanceIndex = m_scene.addInstance(coinGeo);
        coin.position = pos;
        coin.collected = false;
        m_scene.instance(coin.instanceIndex).modelMatrix = makeModel(pos);
        m_coins.push_back(coin);
    }
}

void Game::update(float dt, float halfWidth, bool left, bool right, bool jumpPressed, float currentTime)
{
    m_currentTime = currentTime;
    // Ordine: fisica/collisioni, sincronizzazione grafica, raccolta.
    updatePlayer(dt, halfWidth, left, right, jumpPressed);
    updateModelMatrices();
    collectCoins();
}

void Game::resize(float halfWidth)
{
    // Il terreno si allarga con la finestra; il bordo superiore resta a FLOOR_Y.
    m_scene.instance(m_floorInstance).modelMatrix =
        makeModel({0.0f, FLOOR_Y - 3.0f}, 2.0f * halfWidth, 6.0f);
}

void Game::updateModelMatrices()
{
    m_scene.instance(m_player.instanceIndex).modelMatrix = makeModel(m_player.position);
}

void Game::collectCoins()
{
    const AABB playerBox{m_player.position, {PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.5f}};
    for (std::size_t i = 0; i < m_coins.size(); ++i) {
        Collectible& coin = m_coins[i];
        if (coin.collected) continue;
        // Stessa sinusoide del vertex shader: collisione e immagine coincidono.
        glm::vec2 animatedPosition = coin.position;
        const float phase = static_cast<float>(i) * COIN_PHASE_STEP;
        animatedPosition.y += COIN_BOB_AMPLITUDE *
            std::sin(COIN_BOB_FREQUENCY * m_currentTime + phase);
        const AABB coinBox{animatedPosition, {COIN_RADIUS, COIN_RADIUS}};
        if (intersects(playerBox, coinBox)) {
            coin.collected = true;
            m_scene.instance(coin.instanceIndex).visible = false;
            m_score += 10;
        }
    }
}

void Game::reset()
{
    m_player.position = {-92.0f, FLOOR_Y + PLAYER_HEIGHT * 0.5f};
    m_player.velocity = {0.0f, 0.0f};
    m_player.onGround = true;
    m_score = 0;
    for (Collectible& coin : m_coins) {
        coin.collected = false;
        m_scene.instance(coin.instanceIndex).visible = true;
    }
    updateModelMatrices();
}

// Aggiorna il personaggio in quattro fasi:
// 1) input e salto; 2) movimento orizzontale; 3) gravita'; 4) collisioni verticali.
// La separazione degli assi rende piu' facile capire la risposta alle collisioni.
void Game::updatePlayer(float dt, float halfWidth,
                        bool left, bool right, bool jumpPressed)
{
    Player& p = m_player;
    const float halfPlayerWidth = PLAYER_WIDTH * 0.5f;
    const float halfPlayerHeight = PLAYER_HEIGHT * 0.5f;

    // ==============================================================
    // TODO 2 - MOVIMENTO E SALTO (circa 30 minuti)
    // 1. Ricavare velocity.x dai booleani right e left.
    // 2. Aggiornare position.x usando dt.
    // 3. Limitare la posizione orizzontale con std::clamp.
    // 4. Se jumpPressed && onGround, assegnare JUMP_SPEED a velocity.y.
    // 5. Applicare la gravita' a velocity.y e aggiornare position.y.
    // 6. All'atterraggio sul terreno: correggere y, azzerare vy,
    //    impostare onGround = true.
    // ==============================================================
    (void)dt;
    (void)left;
    (void)right;
    (void)jumpPressed;

    // VERSIONE PROVVISORIA: robot fermo, appoggiato al pavimento.
    // Sostituire questo blocco durante il TODO 2.
    p.velocity = {0.0f, 0.0f};
    p.position.y = FLOOR_Y + halfPlayerHeight;
    p.position.x = std::clamp(p.position.x,
                              -halfWidth + halfPlayerWidth,
                               halfWidth - halfPlayerWidth);
    p.onGround = true;

    // ==============================================================
    // TODO 3 - COLLISION RESPONSE
    // Dopo aver completato TODO 2, aggiungere:
    // A) dopo il moto X, collisioni laterali con m_obstacles;
    // B) dopo il moto Y, collisioni con il pavimento e piattaforme.
    // Se il robot scende sulla piattaforma: correggere y,
    // azzerare velocity.y e impostare onGround = true.
    // Se urta da sotto: correggere y e azzerare velocity.y.
    // ==============================================================
}
