#include "DecorativeShapes.h"

// Figure 2D composte soltanto da triangoli.
// Ogni vertice occupa cinque float: x, y, r, g, b.
namespace {

// Inserisce un triangolo nella mesh, con un unico colore per i tre vertici.
void addTriangle(MeshData& mesh, const glm::vec2& a,
                 const glm::vec2& b, const glm::vec2& c,
                 const glm::vec3& color)
{
    const glm::vec2 points[3] = {a, b, c};
    for (const glm::vec2& p : points) {
        mesh.vertices.insert(mesh.vertices.end(),
                             {p.x, p.y, color.r, color.g, color.b});
    }
}

// Un rettangolo e' formato da due triangoli: ABC e ACD.
void addRectangle(MeshData& mesh, float xMin, float yMin,
                  float xMax, float yMax, const glm::vec3& color)
{
    const glm::vec2 A{xMin, yMin};
    const glm::vec2 B{xMax, yMin};
    const glm::vec2 C{xMax, yMax};
    const glm::vec2 D{xMin, yMax};
    addTriangle(mesh, A, B, C, color);
    addTriangle(mesh, A, C, D, color);
}

} // namespace anonimo

namespace DecorativeShapes {

MeshData robot(float width, float height)
{
    MeshData mesh;
    // Tutte le coordinate sono locali e relative al centro del personaggio.
    // Una sola mesh: la fisica usa ancora PLAYER_WIDTH e PLAYER_HEIGHT.

    // Corpo azzurro: rettangolo centrale.
    addRectangle(mesh, -0.34f*width, -0.25f*height,
                        0.34f*width,  0.13f*height,
                        {0.18f, 0.72f, 0.85f});

    // Testa blu: rettangolo superiore.
    addRectangle(mesh, -0.40f*width, 0.12f*height,
                        0.40f*width, 0.43f*height,
                        {0.13f, 0.47f, 0.72f});

    // Due occhi bianchi, costruiti con due piccoli rettangoli.
    addRectangle(mesh, -0.25f*width, 0.26f*height,
                       -0.10f*width, 0.34f*height,
                        {0.93f, 0.98f, 1.0f});
    addRectangle(mesh,  0.10f*width, 0.26f*height,
                        0.25f*width, 0.34f*height,
                        {0.93f, 0.98f, 1.0f});

    // Piedi scuri, con il bordo inferiore dentro l'AABB del personaggio.
    addRectangle(mesh, -0.33f*width, -0.50f*height,
                       -0.07f*width, -0.25f*height,
                        {0.13f, 0.29f, 0.43f});
    addRectangle(mesh,  0.07f*width, -0.50f*height,
                        0.33f*width, -0.25f*height,
                        {0.13f, 0.29f, 0.43f});
    return mesh;
}

MeshData crystal(float radius)
{
    MeshData mesh;
    // Rombo: quattro vertici e due triangoli, senza dettagli aggiuntivi.
    // Resta contenuto nell'AABB di semilato COIN_RADIUS.
    const glm::vec2 top{0.0f, radius};
    const glm::vec2 right{radius, 0.0f};
    const glm::vec2 bottom{0.0f, -radius};
    const glm::vec2 left{-radius, 0.0f};

    addTriangle(mesh, top, left, bottom, {0.17f, 0.87f, 0.97f});
    addTriangle(mesh, top, bottom, right, {0.06f, 0.56f, 0.88f});
    return mesh;
}

MeshData platformRock()
{
    MeshData mesh;
    // Blocco unitario marrone. La Model Matrix lo scala alla dimensione
    // dell'ostacolo; la collisione AABB rimane quella originale.
    addRectangle(mesh, -0.5f, -0.5f, 0.5f, 0.5f,
                 {0.50f, 0.35f, 0.24f});
    return mesh;
}

MeshData grassStrip()
{
    MeshData mesh;
    // Sottile fascia verde che viene disegnata sopra il blocco.
    // E' solo decorativa e non modifica le collisioni.
    addRectangle(mesh, -0.5f, -0.5f, 0.5f, 0.5f,
                 {0.26f, 0.72f, 0.33f});
    return mesh;
}

} // namespace DecorativeShapes
