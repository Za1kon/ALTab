#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float sharpness;      // 0.0 = sin efecto, 1.0 = máximo
// Opcional: uniform vec2 texelSize; // si prefieres pasar 1/width,1/height desde C++

void main() {
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);

    // 3×3 box blur (promedio)
    vec3 sum = vec3(0.0);
    for(int x=-1; x<=1; ++x){
        for(int y=-1; y<=1; ++y){
            sum += texture(screenTexture, TexCoords + vec2(x, y) * texelSize).rgb;
        }
    }
    vec3 blurred = sum / 9.0;

    // unsharp mask
    vec3 original = texture(screenTexture, TexCoords).rgb;
    vec3 diff = original - blurred;
    vec3 result = original + diff * sharpness;

    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}