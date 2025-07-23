#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

out vec2 vUV;

void main() {
    // Convert position [-size/2, size/2] to [0,1]
    vUV = aPos.xz * 0.5 + 0.5;
    gl_Position = uMVP * vec4(aPos, 1.0);
}