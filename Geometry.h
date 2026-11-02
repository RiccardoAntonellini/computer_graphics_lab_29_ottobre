#pragma once
#include "MeshData.h"
#include <glad/glad.h>
// Possiede VAO e VBO di UNA mesh; piu' istanze possono riutilizzarla.
class Geometry {
    GLuint m_vao=0,m_vbo=0;
    GLsizei m_vertexCount=0;
    GLenum m_primitive=GL_TRIANGLES;
public:
    explicit Geometry(const MeshData& mesh);
    ~Geometry();
    Geometry(const Geometry&)=delete;
    Geometry& operator=(const Geometry&)=delete;
    void draw() const;
};
