#version 330 core
in vec2 vUV;
out vec4 FragColor;

void main() {
    float border = 0.03;

    // If near the edge, draw black
    if (vUV.x < border || vUV.x > 1.0 - border || vUV.y < border || vUV.y > 1.0 - border)
        FragColor = vec4(0.2, 0.2, 0.2, 1.0); // black
    else
        FragColor = vec4(0.5, 0.5, 0.5, 1.0); // gray
}