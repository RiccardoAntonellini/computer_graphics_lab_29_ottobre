#pragma once
#include "MeshData.h"
#include <glm/glm.hpp>
namespace ShapeGenerator {
MeshData rectangle(float width,float height,const glm::vec3& color);
MeshData disk(float radius,const glm::vec3& color,int segments=40);
}
