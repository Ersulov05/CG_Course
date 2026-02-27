#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec4 vertexColor;

void main() {
    gl_Position = uProjection * uTransform * vec4(aPos.x, aPos.y, 0.0, 1.0);
    vertexColor = aColor;
}