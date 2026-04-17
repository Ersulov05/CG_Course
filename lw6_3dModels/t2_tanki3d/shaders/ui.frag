#version 460 core

uniform sampler2D uTextures[2];
uniform int uTextureCount;

in vec4 vertexColor;
in vec2 uvDiffuse;
in vec2 uvShadow;

out vec4 FragColor;

void main() {
    vec4 color;
    if (uTextureCount > 0) {
        color = vertexColor * texture(uTextures[0], uvDiffuse);

        if (uTextureCount > 1) {
            vec4 secondTex = texture(uTextures[1], uvShadow);
            
            float shadowMask = secondTex.r;
            color.rgb *= shadowMask;
        }
    } else {
        color = vertexColor;
    }
    
    FragColor = color;
}