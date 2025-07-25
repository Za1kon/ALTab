#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uScale;

void main() {
    vec4 scaledPos = vec4(aPos * uScale, 1.0);
    gl_Position = uProjection * uView * uModel * scaledPos;
}