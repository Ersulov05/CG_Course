#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

uniform float uTime;
uniform float uScale = 1.0;

void main() {
    vec3 pos = aPos;
    
    // Расширение взрыва
    float expand = 1.0 + uTime * 3.0;
    pos *= expand * uScale;
    
    // Хаотичное смещение
    pos.x += sin(uTime * 15.0 + aPos.y * 8.0) * 0.2 * uTime;
    pos.y += cos(uTime * 18.0 + aPos.z * 8.0) * 0.2 * uTime;
    pos.z += sin(uTime * 20.0 + aPos.x * 8.0) * 0.2 * uTime;
    
    gl_Position = uProjection * uView * uTransform * vec4(pos, 1.0);
}