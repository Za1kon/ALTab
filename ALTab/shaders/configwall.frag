#version 330 core
out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform bool uActive;

void main() {
    vec4 baseColor = texture(uTexture, vTexCoord);
    if (uActive) {
        FragColor = mix(baseColor, vec4(0.2, 1.0, 0.2, 1.0), 0.5);
    } else {
        FragColor = baseColor;
    }
}