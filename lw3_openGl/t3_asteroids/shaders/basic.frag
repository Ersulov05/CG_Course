#version 460 core
in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;  // Текстура для текста
uniform bool uUseTexture = false;     // Флаг: использовать текстуру или цвет

void main() {
    if (uUseTexture) {
        // Для текста: альфа из текстуры, цвет из vertexColor
        float alpha = texture(uTexture, TexCoord).r;
        FragColor = vec4(vertexColor.rgb, vertexColor.a * alpha);
    } else {
        // Для обычной графики
        FragColor = vertexColor;
    }
}
