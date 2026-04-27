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


vec3 planeToMobius(float x, float y) {
    // Отображаем диапазон [-2,2] в параметры ленты
    float u = (x + 2.0) / 4.0 * 2.0 * 3.14159265;  // u: 0..2π
    float v = y / 2.0;                               // v: -1..1
    
    float radius = 1.0;
    float width = 0.8;
    
    float cosU = cos(u);
    float sinU = sin(u);
    float cosHalfU = cos(u * 0.5);
    float sinHalfU = sin(u * 0.5);
    
    float R = radius + v * width * 0.5 * cosHalfU;
    
    return vec3(
        R * cosU,
        R * sinU,
        v * width * 0.5 * sinHalfU
    );
}






vec3 planeToKlein(float x, float y) {
    float u = (x + 2.0) / 4.0 * 2.0 * 3.14159265;
    float v = y / 2.0;
    
    // Параметры для бутылки Клейна
    float R = 1.5;
    float r = 0.5;
    
    float cu = cos(u);
    float su = sin(u);
    float cv = cos(v * 3.14159265);
    float sv = sin(v * 3.14159265);
    
    // Альтернативная, более простая формула бутылки Клейна
    // Которая хорошо погружается в 3D
    float x1 = (R + r * cv) * cu;
    float y1 = (R + r * cv) * su;
    float z1 = r * sv * (1.0 + 0.3 * sin(u));
    
    // Добавляем изгиб для характерной формы
    if (u > 3.14159) {
        float factor = (u - 3.14159) / 3.14159;
        z1 += 0.5 * sin(factor * 3.14159);
        x1 *= (1.0 - factor * 0.3);
        y1 *= (1.0 - factor * 0.3);
    }
    
    // Скейлинг для красивых пропорций
    return vec3(x1 * 0.8, y1 * 0.8, z1 * 0.9);
}

// Другой вариант бутылки Клейна (более классический)
vec3 planeToKleinClassic(float x, float y) {
    float u = (x + 2.0) / 4.0 * 2.0 * 3.14159265;
    float v = y / 1.5;
    
    // Ограничиваем v
    v = clamp(v, -0.8, 0.8);
    
    float u2 = u * 2.0;
    float cosU = cos(u);
    float sinU = sin(u);
    float cosV = cos(v * 3.14159265);
    float sinV = sin(v * 3.14159265);
    
    // Классическая параметризация
    float x1 = (1.5 + cosV * 0.6) * cosU;
    float y1 = (1.5 + cosV * 0.6) * sinU;
    float z1 = sinV * 0.8;
    
    // Добавляем петлю (характерная черта бутылки Клейна)
    if (u > 3.14159) {
        float t = (u - 3.14159) / 3.14159;
        float offset = sin(t * 3.14159) * 0.4;
        z1 += offset;
        x1 *= (1.0 - t * 0.2);
        y1 *= (1.0 - t * 0.2);
    }
    
    // Поворачиваем для красивой ориентации
    return vec3(x1 * 0.9, y1 * 0.8, z1 * 1.2);
}
















// // Преобразование плоскости XOY в бутылку Клейна
// vec3 planeToKlein(float x, float y) {
//     // Отображаем плоскость на параметры бутылки
//     float u = (x + 2.0) / 4.0 * 2.0 * 3.14159265;   // u: 0..2π
//     float v = y / 2.0;                               // v: -1..1
    
//     // Преобразуем v в параметр V для бутылки
//     float V = (v + 1.0) * 3.14159265;                // V: 0..2π
    
//     float a = 1.2;
//     float cosU = cos(u);
//     float sinU = sin(u);
//     float cosV = cos(V);
//     float sinV = sin(V);
//     float cosHalfU = cos(u * 0.5);
//     float sinHalfU = sin(u * 0.5);
    
//     // Основная формула бутылки Клейна
//     float r = a + cosHalfU * sinV - sinHalfU * sin(2.0 * V);
//     vec3 pos = vec3(
//         r * cosU,
//         r * sinU,
//         sinHalfU * sinV + cosHalfU * sin(2.0 * V)
//     );
    
//     // Масштабируем и корректируем форму
//     pos *= 0.7;
    
//     // Добавляем горлышко для v > 0.5
//     if (v > 0.3) {
//         float neck = (v - 0.3) / 0.7;
//         neck = min(neck, 1.0);
//         pos.x *= (1.0 - neck * 0.4);
//         pos.y *= (1.0 - neck * 0.4);
//         pos.z += neck * 0.5;
//     }
    
//     return pos;
// }

// Вычисление нормали для ленты Мёбиуса
vec3 getMobiusNormal(float x, float y) {
    float eps = 0.01;
    vec3 pos = planeToMobius(x, y);
    vec3 dx = planeToMobius(x + eps, y) - planeToMobius(x - eps, y);
    vec3 dy = planeToMobius(x, y + eps) - planeToMobius(x, y - eps);
    return normalize(cross(dx, dy));
}

// Вычисление нормали для бутылки Клейна
vec3 getKleinNormal(float x, float y) {
    float eps = 0.01;
    vec3 pos = planeToKlein(x, y);
    vec3 dx = planeToKlein(x + eps, y) - planeToKlein(x - eps, y);
    vec3 dy = planeToKlein(x, y + eps) - planeToKlein(x, y - eps);
    return normalize(cross(dx, dy));
}


void main() {
    // Получаем фактор морфинга
    float morphFactor = 1.0;
    
    // Вычисляем позиции на обеих поверхностях
    vec3 mobiusPos = planeToMobius(aPos.x, aPos.y);
    vec3 kleinPos = planeToKlein(aPos.x, aPos.y);
    
    // Интерполяция позиций
    vec3 finalPos = mix(mobiusPos, kleinPos, morphFactor);
    
    // Интерполяция нормалей
    vec3 mobiusNormal = getMobiusNormal(aPos.x, aPos.y);
    vec3 kleinNormal = getKleinNormal(aPos.x, aPos.y);
    vec3 finalNormal = normalize(mix(mobiusNormal, kleinNormal, morphFactor));
    
    




    // Преобразования для мира и камеры
    worldPos = vec3(uTransform * vec4(finalPos, 1.0));
    worldNormal = mat3(transpose(inverse(uTransform))) * finalNormal;
    
    vertexColor = aColor;
    
    // Финальная позиция
    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
}
