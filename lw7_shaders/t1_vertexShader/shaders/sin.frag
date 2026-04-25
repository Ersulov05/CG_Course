#version 460 core

uniform vec3 uLightPos;

in vec4 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}