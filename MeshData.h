#pragma once
#include <vector>
#include <glad/glad.h>
// Ogni vertice: x, y, r, g, b.
// La primitiva e' una proprieta' della singola geometria.
struct MeshData {
    std::vector<float> vertices;
    GLenum primitive = GL_TRIANGLES;
};
