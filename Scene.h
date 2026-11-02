#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstddef>
// Una istanza NON contiene una copia dei vertici: conserva un indice e una matrice.
struct GeometryInstance {
    std::size_t geometryIndex=0;
    glm::mat4 modelMatrix{1.0f};
    bool visible=true;
};
class Scene {
    std::vector<std::unique_ptr<Geometry>> m_geometries;
    std::vector<GeometryInstance> m_instances;
public:
    std::size_t addGeometry(const MeshData& mesh);
    std::size_t addInstance(std::size_t geometryIndex);
    GeometryInstance& instance(std::size_t index);
    const Geometry& geometry(std::size_t index) const;
    const std::vector<GeometryInstance>& instances() const;
    void clear();
};
