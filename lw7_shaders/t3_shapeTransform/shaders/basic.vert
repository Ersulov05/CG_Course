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
const float TORUS_MAJOR_RADIUS = 2;
const float TORUS_MINOR_RADIUS = 0.5;

vec3 toSphere(vec2 uv) {
    float theta = uv.x * 3.14159265;
    float phi = uv.y * 3.14159265;
    
    float r = SPHERE_RADIUS;
    
    return vec3(
        r * cos(phi) * cos(theta),
        r * sin(phi),
        r * cos(phi) * sin(theta)
    );
}

vec3 toTorus(vec2 uv) {
    float u = uv.x * 3.14159265;
    float v = uv.y * 3.14159265;
    
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
    // Аналитическое вычисление нормали для тора
    float R = TORUS_MAJOR_RADIUS;
    float r = TORUS_MINOR_RADIUS;
    
    // Находим параметры тора для данной точки
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;
    
    float u = atan(z, x);
    float theta = atan(y, sqrt(x*x + z*z) - R);
    
    // Нормаль для тора
    vec3 normal;
    normal.x = cos(u) * cos(theta);
    normal.y = sin(theta);
    normal.z = sin(u) * cos(theta);
    
    return normal;
}



void main() {    
    vec2 uv = vec2(aPos.x + 1.0, aPos.y + 1.0);
    
    float morfFactor = clamp(uTime, 0.002, 1.0);

    vec3 spherePos = toSphere(uv);
    vec3 torusPos = toTorus(uv);
    vec3 finalPos = mix(spherePos, torusPos, morfFactor);

    vec3 sphereNormal = getSphereNormal(spherePos);
    vec3 torusNormal = getTorusNormal(torusPos);
    vec3 finalNormal = mix(sphereNormal, torusNormal, morfFactor);
    finalNormal = normalize(finalNormal);

    worldPos = vec3(uTransform * vec4(finalPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * finalNormal;
    
    vertexColor = aColor;
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
