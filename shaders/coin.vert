#version 330 core
// Shader dedicato esclusivamente alle monete.
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;
uniform mat4 model;
uniform float uTime;
uniform float uAmplitude;
uniform float uFrequency;
uniform float uPhase;
out vec3 vColor;

void main()
{
    // La Model Matrix colloca la moneta nella posizione di riposo.
    vec4 worldPosition = model * vec4(aPosition, 0.0, 1.0);
    // Oscillazione verticale in UNITA' DEL MONDO, dopo la Model Matrix.
    worldPosition.y += uAmplitude * sin(uFrequency * uTime + uPhase);
    gl_Position = projection * worldPosition;
    vColor = aColor;
}
