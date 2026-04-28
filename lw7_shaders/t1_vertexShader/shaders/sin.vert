#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec4 vertexColor;

void main() {
    vertexColor = aColor;

    vec3 modifiedPos = aPos;

    // R=(1+sinx)(1+0,9cos8x)(1+0,1cos24x)(0,5+0,05cos140x)
    float r = (1 + sin(aPos.x)) *
        (1 + 0.9* cos(8*aPos.x)) *
        (1 + 0.1*cos(24*aPos.x)) *
        (0.5+0.05*cos(140 * aPos.x));

    modifiedPos.x = r * cos(aPos.x);
    modifiedPos.y = r * sin(aPos.x);
    modifiedPos.z = aPos.z;

    gl_Position = uProjection * uView * uTransform * vec4(modifiedPos, 1.0);
}
