#version 330 core
out vec4 FragColor;

uniform vec3 uBaseColor;
uniform bool uMicroshot;

void main() {
    vec3 color = uBaseColor;
    if (uMicroshot) {
        color = vec3(1.0, 0.5, 0.5);
    }
    FragColor = vec4(color, 1.0);
}