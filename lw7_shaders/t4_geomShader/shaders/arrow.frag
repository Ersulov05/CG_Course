#version 460 core

in vec4 geometryColor;

out vec4 FragColor;

void main() {
    FragColor = geometryColor;
}