#include "ShapeGenerator.h"
#include <array>
#include <cmath>
namespace ShapeGenerator {
MeshData rectangle(float w,float h,const glm::vec3& color) {
    const float x=w*0.5f,y=h*0.5f;
    const std::array<glm::vec2,6> pts={glm::vec2(-x,-y),{x,-y},{x,y},{-x,-y},{x,y},{-x,y}};
    MeshData mesh;
    for(const auto& v:pts) mesh.vertices.insert(mesh.vertices.end(),{v.x,v.y,color.r,color.g,color.b});
    return mesh;
}
MeshData disk(float r,const glm::vec3& color,int segments) {
    MeshData mesh;
    const float pi=3.14159265358979323846f;
    for(int i=0;i<segments;++i) {
        const float a=2*pi*i/segments, b=2*pi*(i+1)/segments;
        const std::array<glm::vec2,3> tri={glm::vec2(0),{r*std::cos(a),r*std::sin(a)},{r*std::cos(b),r*std::sin(b)}};
        for(const auto& v:tri) mesh.vertices.insert(mesh.vertices.end(),{v.x,v.y,color.r,color.g,color.b});
    }
    return mesh;
}
}
