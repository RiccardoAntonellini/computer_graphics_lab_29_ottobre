#include "DecorativeShapes.h"
#include <cmath>
namespace {
void triangle(MeshData& m, glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 col) {
    for (const auto& v : {a,b,c}) m.vertices.insert(m.vertices.end(), {v.x,v.y,col.r,col.g,col.b});
}
void rect(MeshData& m, float x0,float y0,float x1,float y1,glm::vec3 col) {
    triangle(m,{x0,y0},{x1,y0},{x1,y1},col);
    triangle(m,{x0,y0},{x1,y1},{x0,y1},col);
}
}
namespace DecorativeShapes {
MeshData robot(float w,float h) {
    MeshData m;
    // Corpo, testa, schermo, occhi e piedi: una singola mesh non animata.
    rect(m,-.36f*w,-.20f*h,.36f*w,.22f*h,{.16f,.68f,.76f});
    rect(m,-.43f*w,.13f*h,.43f*w,.46f*h,{.11f,.43f,.63f});
    rect(m,-.33f*w,.20f*h,.33f*w,.37f*h,{.04f,.14f,.27f});
    rect(m,-.23f*w,.26f*h,-.10f*w,.32f*h,{.55f,1.f,.98f});
    rect(m,.10f*w,.26f*h,.23f*w,.32f*h,{.55f,1.f,.98f});
    rect(m,-.40f*w,-.49f*h,-.08f*w,-.32f*h,{.12f,.28f,.42f});
    rect(m,.08f*w,-.49f*h,.40f*w,-.32f*h,{.12f,.28f,.42f});
    rect(m,-.49f*w,-.13f*h,-.37f*w,.13f*h,{.23f,.82f,.86f});
    rect(m,.37f*w,-.13f*h,.49f*w,.13f*h,{.23f,.82f,.86f});
    rect(m,-.05f*w,.46f*h,.05f*w,.55f*h,{.22f,.74f,.85f});
    rect(m,-.10f*w,.52f*h,.10f*w,.57f*h,{1.f,.83f,.30f});
    return m;
}
MeshData crystal(float r) {
    MeshData m;
    // Cristallo sfaccettato con sagoma romboidale, centrato nell'origine.
    glm::vec2 top{0.f,r}, left{-.72f*r,.15f*r}, right{.72f*r,.15f*r}, bottom{0.f,-r};
    triangle(m,top,left,{-.13f*r,.10f*r},{.25f,.98f,1.f});
    triangle(m,top,{-.13f*r,.10f*r},right,{.10f,.66f,.96f});
    triangle(m,left,bottom,{-.13f*r,.10f*r},{.06f,.45f,.80f});
    triangle(m,{- .13f*r,.10f*r},bottom,right,{.10f,.78f,.92f});
    triangle(m,{-.28f*r,.25f*r},{-.17f*r,.50f*r},{-.05f*r,.32f*r},{.85f,1.f,1.f});
    return m;
}
MeshData platformRock() {
    MeshData m;
    // Rettangolo unitario, che mantiene la AABB fisica invariata.
    rect(m,-.5f,-.5f,.5f,.5f,{.37f,.40f,.47f});
    triangle(m,{-.5f,-.5f},{-.12f,-.5f},{-.3f,.22f},{.28f,.34f,.43f});
    triangle(m,{.12f,-.5f},{.5f,-.5f},{.32f,.24f},{.30f,.35f,.43f});
    triangle(m,{-.35f,.1f},{.0f,-.13f},{.20f,.16f},{.46f,.49f,.53f});
    return m;
}
MeshData grassStrip() {
    MeshData m;
    rect(m,-.5f,-.5f,.5f,.30f,{.19f,.65f,.32f});
    rect(m,-.5f,-.5f,.5f,-.15f,{.11f,.47f,.23f});
    for(int i=0;i<9;++i) {
        float x=-.46f+i*.115f;
        triangle(m,{x,.25f},{x+.035f,.53f},{x+.075f,.25f},{.35f,.80f,.36f});
    }
    return m;
}
}
