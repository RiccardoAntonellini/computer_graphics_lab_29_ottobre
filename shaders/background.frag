#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform float uTime;

void main()
{
    // Cielo sfumato, piu' chiaro vicino all'orizzonte.
    vec3 horizon = vec3(0.70, 0.88, 0.98);
    vec3 zenith  = vec3(0.07, 0.24, 0.48);
    vec3 color = mix(horizon, zenith, smoothstep(0.0, 1.0, vUV.y));

    // Nuvole molto leggere: due bande ondulate che scorrono lentamente.
    float x = vUV.x + uTime * 0.012;
    float band1 = exp(-pow((vUV.y - (0.76 + 0.025 * sin(x * 15.0))) * 23.0, 2.0));
    float band2 = exp(-pow((vUV.y - (0.58 + 0.020 * sin(x * 11.0 + 1.7))) * 30.0, 2.0));
    float cloud = 0.14 * band1 + 0.09 * band2;
    color = mix(color, vec3(0.93, 0.96, 1.0), clamp(cloud, 0.0, 1.0));

    FragColor = vec4(color, 1.0);
}
