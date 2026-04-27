#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

out vec3 worldPos;
out vec3 worldNormal;
out vec4 vertexColor;


vec3 toSphere(vec2 uv) {
    float theta = uv.x * 3.14159265;           // 0..π
    float phi = uv.y * 2.0 * 3.14159265;       // 0..2π
    float r = 1.5;
    
    return vec3(
        r * sin(theta) * cos(phi),
        r * sin(theta) * sin(phi),
        r * cos(theta)
    );
}

vec3 toTorus(vec2 uv) {
    float u = uv.x * 2.0 * 3.14159265;         // 0..2π
    float v = uv.y * 2.0 * 3.14159265;         // 0..2π
    float R = 1.5;
    float r = 0.5;
    
    return vec3(
        (R + r * cos(v)) * cos(u),
        (R + r * cos(v)) * sin(u),
        r * sin(v)
    );
}



void main() {
    // Получаем фактор морфинга
    float morphFactor = 1.0;
    
    vec2 uv = vec2(
        (aPos.x + 2.0) / 4.0,   // 0..1
        (aPos.y + 2.0) / 4.0    // 0..1
    );

    
    

    vec3 spherePos = toSphere(uv);
    vec3 torusPos = toTorus(uv);
    
    // Интерполяция
    vec3 finalPos = mix(spherePos, torusPos, morphFactor);
    
    // Упрощённая нормаль (для демонстрации)
    vec3 finalNormal = normalize(finalPos);



    // Преобразования для мира и камеры
    worldPos = vec3(uTransform * vec4(finalPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * finalNormal;
    
    vertexColor = aColor;
    
    // Финальная позиция
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
