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

vec3 mobiusToKlein(vec3 mobiusPos, float t) {
    // Получаем цилиндрические координаты
    float r = length(mobiusPos.xy);
    float angle = atan(mobiusPos.y, mobiusPos.x);
    float height = mobiusPos.z;
    
    // Нормализуем параметры
    float u = angle;
    if (u < 0.0) u += 2.0 * 3.14159;
    
    // По высоте определяем v (упрощённо)
    float v = height * 2.0; // примерная оценка
    
    // Преобразование в бутылку Клейна (эмпирическая формула)
    float R = 1.5;
    float kleinX = R * cos(u) * (1.0 + 0.5 * cos(v * 3.14159));
    float kleinY = R * sin(u) * (1.0 + 0.5 * cos(v * 3.14159));
    float kleinZ = 0.8 * sin(v * 3.14159) + 0.3 * sin(u);
    
    // Добавляем характерную "петлю" бутылки
    if (u > 3.14159) {
        kleinZ += 0.5 * sin((u - 3.14159) * 2.0);
    }
    
    return vec3(kleinX, kleinY, kleinZ);
}


void main() {
    // Плавный переход
    float t = 0.0; //clamp(uTime, 0.0, 1.0);
    
    // Получаем позицию бутылки из позиции ленты
    vec3 kleinPos = mobiusToKlein(aPos, t);
    
    // Интерполяция между исходной позицией и бутылкой
    vec3 finalPos = mix(aPos, kleinPos, t);
    
    // // Добавляем эффект "скручивания" в середине анимации
    // if (t > 0.3 && t < 0.7) {
    //     float twistFactor = sin((t - 0.3) * 3.14159 / 0.4);
    //     float angle = atan(finalPos.y, finalPos.x);
    //     float r = length(finalPos.xy);
    //     angle += twistFactor * 0.5;
    //     finalPos.x = r * cos(angle);
    //     finalPos.y = r * sin(angle);
    // }
    
    worldPos = vec3(uTransform * vec4(finalPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * aNormal;
    
    vertexColor = aColor;
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}



// void main() {
//     worldPos = vec3(uTransform * vec4(aPos, 1.0));
//     worldNormal = mat3(transpose(inverse(uTransform))) * aNormal;
//     vertexColor = aColor;
//     gl_Position = uProjection * uView * vec4(worldPos, 1.0);
// }
