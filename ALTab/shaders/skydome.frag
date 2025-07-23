// ===== shaders/skydome.frag =====
#version 330 core
in vec3 vDir;
out vec4 FragColor;
uniform sampler2D uHDR;
void main() {
    vec3 dir = normalize(vDir);
    float phi = atan(dir.z, dir.x);
    float theta = asin(clamp(dir.y, -1.0, 1.0));
    vec2 uv = vec2(phi / (2.0 * 3.1415926) + 0.5,
                   theta / 3.1415926 + 0.5);
    vec3 hdrColor = texture(uHDR, uv).rgb;
    vec3 color = clamp(hdrColor, 0.0, 1.0);
    FragColor = vec4(color, 1.0);
}
