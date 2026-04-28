#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 aUVDiffuse;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec2 uv;

void main() {
    uv = aUVDiffuse;
    gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
}
