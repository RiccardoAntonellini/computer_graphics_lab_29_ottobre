#include "Landscape.h"
#include "ShapeGenerator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>
// ======================================================
// FUNZIONI AUSILIARIE DI LANDSCAPE.CPP
//
// Il namespace anonimo contiene funzioni utilizzate
// soltanto all'interno di questo file.
//
// Non sono metodi della classe Landscape.
// Non fanno parte della sua interfaccia pubblica.
// ======================================================

namespace {
    // Una mesh e' una sequenza di triangoli non indicizzati.
    void appendVertex(MeshData& mesh, const glm::vec2& p, const glm::vec3& c) {
        mesh.vertices.insert(mesh.vertices.end(), {p.x, p.y, c.r, c.g, c.b});
    }
    void appendTriangle(MeshData& mesh, const glm::vec2& a,
                        const glm::vec2& b, const glm::vec2& c,
                        const glm::vec3& color) {
        appendVertex(mesh, a, color);
        appendVertex(mesh, b, color);
        appendVertex(mesh, c, color);
    }
    glm::mat4 model(const glm::vec2& center, const glm::vec2& size) {
        return glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) *
               glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    }
}

// Calcola y(x) sul segmento del profilo che contiene x.
// E' l'interpolazione lineare: y = y0 + t*(y1-y0).
float Landscape::heightOnSlope(const std::vector<glm::vec2>& profile, float x) {
    for (std::size_t i = 1; i < profile.size(); ++i) {
        const auto& a = profile[i-1];
        const auto& b = profile[i];
        if (x >= a.x && x <= b.x) {
            const float t = (x - a.x) / (b.x - a.x);
            return a.y + t * (b.y - a.y);
        }
    }
    return profile.front().y;
}

// Genera una superficie sinusoidale nel riferimento locale.
// Sequenza: base_0, cima_0, base_1, cima_1, ...
// GL_TRIANGLE_STRIP costruisce automaticamente 2*segments triangoli.
std::vector<glm::vec2> Landscape::addMountain(
    float width, float height, float amplitude,
    float frequency, float phase, int segments,
    const glm::vec2& position, const glm::vec3& color)
{
    std::vector<glm::vec2> profile;
    if (segments < 1 || width <= 0.0f || height <= amplitude || amplitude < 0.0f)
        return profile;

    MeshData mesh;
    mesh.primitive = GL_TRIANGLE_STRIP;
    profile.reserve(static_cast<std::size_t>(segments + 1));

    // ==============================================================
    // TODO 1 - GEOMETRIA PROCEDURALE (circa 40 minuti)
    // Obiettivo: campionare y = height + amplitude*sin(frequency*x+phase)
    // e costruire una GL_TRIANGLE_STRIP con la sequenza:
    // (x0,0), (x0,y0), (x1,0), (x1,y1), ...
    // Ogni punto (x,y) va anche salvato in profile per gli alberi.
    //
    // DA COMPLETARE IN AULA: sostituire il profilo provvisorio
    // orizzontale con il campionamento della sinusoide.
    // ==============================================================
    for (int i = 0; i <= segments; ++i) {
        float x = width * static_cast<float>(i) / static_cast<float>(segments);
        float y = height;  // TODO 1: calcolare l'ordinata sinusoidale

        profile.push_back({x, y});
        appendVertex(mesh, {x, 0.0f}, color);
        appendVertex(mesh, {x, y}, color);
    }

    std::size_t geometry = m_scene.addGeometry(mesh);
    std::size_t instance = m_scene.addInstance(geometry);
    m_scene.instance(instance).modelMatrix =
        glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    return profile;
}

// Un albero e' formato da 1 istanza di tronco + 2 istanze
// della stessa chioma triangolare (due triangoli sovrapposti).
void Landscape::addTree(float x, float groundY, float size,
                        std::size_t trunkGeometry, std::size_t crownGeometry) {
    const float trunkH = 5.5f * size;
    const float trunkW = 1.35f * size;
    const auto trunk = m_scene.addInstance(trunkGeometry);
    m_scene.instance(trunk).modelMatrix =
        model({x, groundY + trunkH * 0.5f}, {trunkW, trunkH});

    const auto lower = m_scene.addInstance(crownGeometry);
    m_scene.instance(lower).modelMatrix =
        model({x, groundY + trunkH + 1.5f * size}, {8.0f * size, 8.0f * size});
    const auto upper = m_scene.addInstance(crownGeometry);
    m_scene.instance(upper).modelMatrix =
        model({x, groundY + trunkH + 5.0f * size}, {6.0f * size, 7.0f * size});
}

Landscape::Landscape() {
    // Le tre montagne condividono la stessa base nel mondo.
    const glm::vec2 mountainPosition(-160.0f, -65.0f);
    const float width = 320.0f;
    const int segments = 160; // passo orizzontale = 2 unita'

    addMountain(width, 60.0f, 25.0f, 0.08f, 0.0f, segments,
                mountainPosition, {0.35f, 0.52f, 0.69f});
    addMountain(width, 40.0f, 15.0f, 0.10f, 0.8f, segments,
                mountainPosition, {0.23f, 0.48f, 0.55f});
    // Salviamo i campioni della collina vicina per appoggiarvi gli alberi.
    const std::vector<glm::vec2> nearProfile =
        addMountain(width, 26.0f, 8.0f, 0.12f, 1.5f, segments,
                    mountainPosition, {0.17f, 0.40f, 0.34f});

    // Una sola mesh rettangolare per TUTTI i tronchi.
    const std::size_t trunkGeo = m_scene.addGeometry(
        ShapeGenerator::rectangle(1.0f,1.0f,{0.33f,0.23f,0.17f}));

    // Una sola mesh triangolare per TUTTE le chiome.
    MeshData crown;
    const glm::vec3 green(0.10f,0.31f,0.25f);
    appendTriangle(crown, {-0.5f,-0.5f}, {0.5f,-0.5f}, {0.0f,0.5f}, green);
    const std::size_t crownGeo = m_scene.addGeometry(crown);

    // Alberi posizionati sulla collina vicina mediante y(x).
    // Le posizioni evitano una disposizione perfettamente regolare.
    const std::vector<float> treeX = {
        -130,-117,-103,-91,-77,-63,-48,-34,-20,-7,
        9,23,37,51,66,80,95,111,127
    };
    for (std::size_t i=0; i<treeX.size(); ++i) {
        const float scale = 0.72f + 0.13f * static_cast<float>(i%4);
        // L'interpolazione usa le coordinate LOCALI della collina.
        float localX = treeX[i] - mountainPosition.x;
        float localY = heightOnSlope(nearProfile, localX);
        float worldY = mountainPosition.y + localY;
        addTree(treeX[i], worldY, scale, trunkGeo, crownGeo);
    }
}

void Landscape::draw(const Renderer& renderer) const {
    renderer.drawScene(m_scene);
}
void Landscape::clear() { m_scene.clear(); }
