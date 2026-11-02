#pragma once
#include "MeshData.h"
#include <glm/glm.hpp>
namespace DecorativeShapes {
// Tutti i vertici sono in coordinate locali; una sola Geometry per figura.
MeshData robot(float width, float height);
MeshData crystal(float radius);
MeshData platformRock();
MeshData grassStrip();
}
