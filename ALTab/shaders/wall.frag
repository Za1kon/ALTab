#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform vec3 bgColor;
uniform vec3 lineColor;
uniform vec3 accentColor;
uniform float borderSize;
uniform vec2 gridCount;
uniform float lineWidth;
uniform float glowWidth;

void main() {
    // borde exterior
    if (vUV.x < borderSize || vUV.x > 1.0-borderSize ||
        vUV.y < borderSize || vUV.y > 1.0-borderSize) {
        FragColor = vec4(bgColor * 0.5, 1.0);
        return;
    }

    // normalizar al área útil (sin bordes)
    vec2 uv2 = (vUV - vec2(borderSize)) / (1.0 - 2.0*borderSize);
    // generar el tile index y fract
    vec2 f = fract(uv2 * gridCount);

    // líneas de grid
    float lx = smoothstep(0.0, lineWidth, f.x) 
             + smoothstep(1.0 - lineWidth, 1.0, f.x);
    float ly = smoothstep(0.0, lineWidth, f.y) 
             + smoothstep(1.0 - lineWidth, 1.0, f.y);
    float lines = clamp(lx + ly, 0.0, 1.0);

    // glow (opcional)
    float gx = smoothstep(lineWidth + glowWidth, lineWidth, abs(f.x-0.5));
    float gy = smoothstep(lineWidth + glowWidth, lineWidth, abs(f.y-0.5));
    float glow = clamp(max(gx, gy), 0.0, 1.0);

    // color final
    vec3 color = mix(bgColor, accentColor, 0.2);
    color = mix(color, lineColor, lines);
    color += lineColor * glow * 0.3;

    FragColor = vec4(color, 1.0);
}