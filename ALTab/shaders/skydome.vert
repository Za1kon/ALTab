// ===== shaders/skydome.vert =====
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
out vec3 vDir;
void main() {
    vDir = aPos;
    gl_Position = uMVP * vec4(aPos, 1.0);
}