#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aUVDiffuse;
layout (location = 4) in vec2 aUVShadow;

uniform mat4 uTransform;
uniform mat4 uProjection;

out vec4 vertexColor;
out vec2 uvDiffuse;
out vec2 uvShadow;

void main() {
    vertexColor = aColor;
    uvDiffuse = aUVDiffuse;
    uvShadow = aUVShadow;

    gl_Position = uProjection * uTransform * vec4(aPos, 1.0);
}
