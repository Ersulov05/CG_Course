#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;  // Добавляем текстурные координаты

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec4 vertexColor;
out vec2 TexCoord;  // Передаем в фрагментный шейдер

void main() {
    gl_Position = uProjection * uTransform * vec4(aPos.x, aPos.y, 0.0, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}