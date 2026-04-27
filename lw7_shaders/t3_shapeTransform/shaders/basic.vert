#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;
uniform float uTime;

out vec3 worldPos;
out vec3 worldNormal;
out vec4 vertexColor;

const float SPHERE_RADIUS = 2;
const float TORUS_MAJOR_RADIUS = 3;
const float TORUS_MINOR_RADIUS = 1;
const float M_PI = 3.1415926;

float getFactor() {
    return clamp(uTime, 0.0, 1.0);
}

vec3 getSpherePos(vec2 uv) {
    float theta = uv.x * M_PI;
    float phi = uv.y * M_PI;
    
    float r = SPHERE_RADIUS;
    
    return vec3(
        r * cos(phi) * cos(theta),
        r * sin(phi),
        r * cos(phi) * sin(theta)
    );
}

vec3 getTorPos(vec2 uv) {
    float u = uv.x * M_PI;
    float v = uv.y * M_PI;
    
    float R = TORUS_MAJOR_RADIUS;
    float r = TORUS_MINOR_RADIUS;
    
    return vec3(
        (R + r * cos(v)) * cos(u),
        r * sin(v),
        (R + r * cos(v)) * sin(u)
    );
}

vec3 getSphereNormal(vec3 pos) {
    return normalize(pos);
}

vec3 getTorusNormal(vec3 pos) {
    float R = TORUS_MAJOR_RADIUS;
    float r = TORUS_MINOR_RADIUS;
    
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;
    
    float u = atan(z, x);
    float theta = atan(y, sqrt(x*x + z*z) - R);
    
    vec3 normal;
    normal.x = cos(u) * cos(theta);
    normal.y = sin(theta);
    normal.z = sin(u) * cos(theta);
    
    return normal;
}

void main() {    
    vec2 uv = vec2(aPos.x + 1.0, aPos.y + 1.0);
    float factor = getFactor();

    vec3 spherePos = getSpherePos(uv);
    vec3 torusPos = getTorPos(uv);
    
    vec3 sphereNormal = getSphereNormal(spherePos);
    vec3 torusNormal = getTorusNormal(torusPos);

    vec3 finalPos = mix(spherePos, torusPos, factor);
    vec3 finalNormal = normalize(mix(sphereNormal, torusNormal, factor));

    worldPos = vec3(uTransform * vec4(finalPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * finalNormal;
    
    vertexColor = aColor;
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
