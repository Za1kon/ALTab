#version 330 core
in vec2 vUV;
out vec4 FragColor;

// Colores base
uniform vec3 baseColor      = vec3(0.4, 0.4, 0.45);
uniform vec3 edgeColor      = vec3(0.1, 0.1, 0.1);
uniform vec3 patternColor   = vec3(0.6, 0.6, 0.7);

uniform float edgeSmoothness = 0.03;
uniform float noiseScale      = 80.0;
uniform float patternStrength = 0.2;

// Hash noise
float hash(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

void main() {
    // --- coordenadas centradas ---
    vec2 centeredUV = vUV - vec2(0.5);
    float dist = length(centeredUV);

    // --- máscara circular (radio = 0.5) ---
    if (dist > 0.5)
        discard;  // fuera del círculo

    // --- borde suave hacia afuera ---
    float edge = smoothstep(0.5, 0.5 - edgeSmoothness, dist);

    // --- patrón procedural ---
    float n = hash(floor(vUV * noiseScale));
    vec3 pattern = mix(baseColor, patternColor, n * patternStrength);

    // --- mezcla final ---
    vec3 color = mix(pattern, edgeColor, edge);

    // --- alfa radial (transparencia) ---
    float alpha = smoothstep(0.5, 0.4, dist);  // de 1.0 en el centro a 0.0 en el borde

    // --- color final con alfa ---
    FragColor = vec4(color, alpha);
}