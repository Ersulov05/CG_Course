#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec3 worldPos;
out vec3 worldNormal;
out vec4 vertexColor;
out vec2 texCoord;

void main() {
    worldPos = vec3(uTransform * vec4(aPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * aNormal;
    vertexColor = aColor;
    texCoord = aTexCoord;
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
