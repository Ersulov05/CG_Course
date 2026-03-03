#version 460 core
in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uUseTexture = false;

void main() {
    if (uUseTexture) {
        float alpha = texture(uTexture, TexCoord).r;
        FragColor = vec4(vertexColor.rgb, vertexColor.a * alpha);
    } else {
        FragColor = vertexColor;
    }
}
