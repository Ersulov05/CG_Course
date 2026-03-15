#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec4 vertexColor;

void main() {
    gl_Position = uProjection * uView  * uTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
}