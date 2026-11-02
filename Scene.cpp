#include "Scene.h"
#include <stdexcept>

// Geometry possiede i buffer GPU; l'indice restituito identifica la mesh.
std::size_t Scene::addGeometry(const MeshData& mesh)
{
    m_geometries.push_back(std::make_unique<Geometry>(mesh));
    return m_geometries.size() - 1;
}

// Una istanza riutilizza la Geometry: conserva solo indice, matrice e visibilita'.
std::size_t Scene::addInstance(std::size_t geometryIndex)
{
    if (geometryIndex >= m_geometries.size()) {
        throw std::out_of_range("geometryIndex non valido");
    }
    m_instances.push_back({geometryIndex, glm::mat4(1.0f), true});
    return m_instances.size() - 1;
}

GeometryInstance& Scene::instance(std::size_t index)
{
    return m_instances.at(index);
}

const Geometry& Scene::geometry(std::size_t index) const
{
    return *m_geometries.at(index);
}

const std::vector<GeometryInstance>& Scene::instances() const
{
    return m_instances;
}

void Scene::clear()
{
    m_instances.clear();
    m_geometries.clear();
}
