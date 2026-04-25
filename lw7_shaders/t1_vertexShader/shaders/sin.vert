#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aUVDiffuse;
layout (location = 4) in vec2 aUVShadow;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec4 vertexColor;
const float AMPLITUDE = 4;

void main() {
    vertexColor = aColor;

    vec3 modifiedPos = aPos;
    modifiedPos.y = AMPLITUDE * sin(aPos.x) / aPos.x;

    gl_Position = uProjection * uView * uTransform * vec4(modifiedPos, 1.0);
}
