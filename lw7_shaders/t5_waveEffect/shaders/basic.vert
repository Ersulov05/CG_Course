#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aUVDiffuse;

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec3 worldNormal;
out vec4 vertexColor;
out vec2 uv;

void main() {
    vertexColor = aColor;
    uv = aUVDiffuse;
    gl_Position = uProjection * uTransform * vec4(aPos, 1.0);
}
