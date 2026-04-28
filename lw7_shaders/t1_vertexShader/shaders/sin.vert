#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;

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
