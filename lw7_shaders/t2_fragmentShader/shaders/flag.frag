#version 460 core
in vec2 uv;

out vec4 FragColor;

float sign(vec2 p1, vec2 p2, vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

float toRad(float angle)
{
    return angle / 180 * 3.1415926;
}

vec2 rotate(vec2 p, float cosA, float sinA) 
{
    return vec2(
        p.x * cosA - p.y * sinA,
        p.x * sinA + p.y * cosA
    );
}

vec2 rotateOnAngle(vec2 p, float angle) 
{
    float radAngle = toRad(angle);
    return rotate(p, cos(radAngle), sin(radAngle));
}

float circle(vec2 uv, vec2 center, float radius)
{
    return length(uv - center) <= radius ? 1.0 : 0.0;
}

float triangle(vec2 uv, vec2 p1, vec2 p2, vec2 p3)
{
    float d1 = sign(uv, p1, p2);
    float d2 = sign(uv, p2, p3);
    float d3 = sign(uv, p3, p1);
    
    bool has_neg = (d1 < 0.0) || (d2 < 0.0) || (d3 < 0.0);
    bool has_pos = (d1 > 0.0) || (d2 > 0.0) || (d3 > 0.0);
    
    return (has_neg && has_pos) ? 0.0 : 1.0;
}

float rect(vec2 uv, vec2 center, float width, float height, float angle = 0)
{
    float cosA = cos(toRad(angle));
    float sinA = sin(toRad(angle));
    
    float halfWidth = width / 2.0;
    float halfHeight = height / 2.0;
    
    vec2 localLeftTop = vec2(-halfWidth,  -halfHeight);
    vec2 localRightTop = vec2( halfWidth,  -halfHeight);
    vec2 localLeftBottom = vec2(-halfWidth, halfHeight);
    vec2 localRightBottom = vec2( halfWidth, halfHeight);
    
    vec2 leftTop = center + rotate(localLeftTop, cosA, sinA);
    vec2 rightTop = center + rotate(localRightTop, cosA, sinA);
    vec2 leftBottom = center + rotate(localLeftBottom, cosA, sinA);
    vec2 rightBottom = center + rotate(localRightBottom, cosA, sinA);
    
    float triangle1 = triangle(uv, leftTop, leftBottom, rightBottom);
    float triangle2 = triangle(uv, rightBottom, rightTop, leftTop);
    
    return max(triangle1, triangle2);
}

float hammerHandle(vec2 uv, vec2 center, float handleLength, float handleWidth, float angle)
{   
    return rect(uv, center, handleWidth, handleLength, angle);
}

float hammerHead(vec2 uv, vec2 center, float width, float height, float angle)
{
    float cosA = cos(toRad(angle));
    float sinA = sin(toRad(angle));
    
    float halfWidth = width / 2.0;
    float halfHeight = height / 2.0;
    
    vec2 localLeftTop = vec2(-halfWidth,  -halfHeight);
    vec2 localRightTop = vec2( halfWidth,  -halfHeight);
    vec2 localLeftBottom = vec2(-halfWidth, halfHeight);
    vec2 localRightBottom = vec2( halfWidth, halfHeight);
    
    vec2 leftTop = center + rotate(localLeftTop, cosA, sinA);
    vec2 rightTop = center + rotate(localRightTop, cosA, sinA);
    vec2 leftBottom = center + rotate(localLeftBottom, cosA, sinA);
    vec2 rightBottom = center + rotate(localRightBottom, cosA, sinA);
    vec2 rightBottom2 = center + rotate(localRightBottom + vec2(halfWidth/4, 0), cosA, sinA);
    
    float triangle1 = triangle(uv, leftTop, leftBottom, rightBottom);
    float triangle2 = triangle(uv, rightBottom, rightTop, leftTop);
    float triangle3 = triangle(uv, rightTop, rightBottom, rightBottom2);
    
    return max(triangle1, max(triangle2, triangle3));
}

vec2 getHeadCenter(float handleLength, float angle)
{
    return rotateOnAngle(vec2(0, -handleLength/2), angle);
}

float hammer(vec2 uv, vec2 center, float angle) 
{
    float handleLength = 110;
    float handleWidth = 8;
    float headWidth = 30;
    float headHeight = 15;
    float handleOffset = 12;

    center.x += handleOffset * cos(toRad(-angle));
    center.y += handleOffset * sin(toRad(-angle));

    vec2 headCenter = center + getHeadCenter(handleLength, angle);
    float hammerHandle = hammerHandle(uv, center, handleLength, handleWidth, angle);
    float hammerHead = hammerHead(uv, headCenter, headWidth, headHeight, angle);

    return max(hammerHandle, hammerHead);
}

float scytheSubtract(vec2 uv, vec2 center, float radius, float thickness, float angle)
{    
    vec2 offset = vec2(-radius*0.02, -radius * 0.15);
    vec2 subtractCenter = center + rotateOnAngle(offset, angle);

    float mainCircle = circle(uv, center, radius);
    float subtractCircle = circle(uv, subtractCenter, radius - thickness);
    
    return mainCircle * (1.0 - subtractCircle);
}

float sickle(vec2 uv, vec2 center, float angle)
{
    float handleWidth = 24;
    float handleHeight = 4;
    vec2 handleCenter = center + rotateOnAngle(vec2(-50 - handleWidth/2, 0), angle);  

    float sickleHandle = rect(uv, handleCenter, handleWidth, handleHeight, angle);
    float scytheShape = scytheSubtract(uv, center, 50, 1, angle);

    return max(scytheShape, sickleHandle);
}

float star(vec2 uv, vec2 center, float radius) {
    vec2 pUp = vec2(0, -radius);
    vec2 pLeft = rotateOnAngle(pUp, -72);
    vec2 pRight = rotateOnAngle(pUp, 72);
    vec2 pBottomLeft = rotateOnAngle(pUp, -144);
    vec2 pBottomRight = rotateOnAngle(pUp, 144);
    vec2 pCenter = vec2(0, -pLeft.y);

    float triangle1 = triangle(uv, center + pCenter, center + pLeft, center + pRight);
    float triangle2 = triangle(uv, center + pUp, center + pBottomLeft, center + pBottomRight);
    float triangle3 = triangle(uv, center + pCenter, center + pBottomLeft, center + pBottomRight);
    return max(triangle1, triangle2) * (1.0 - triangle3);
}

float starBorder(vec2 uv, vec2 center, float radius, float size)
{
    return star(uv, center, radius) * (1.0 - star(uv, center, radius - size));
}

const float STAR_RADIUS = 16;
const float STAR_SIZE = 4;
const vec2 STAR_OFFSET = vec2(0, -70);
const vec3 RED_COLOR = vec3(0.788, 0.102, 0.102);
const vec3 YELLOW_COLOR = vec3(1.0, 0.843, 0.0);
const vec2 SUMBOL_CENTER = vec2(100.0, 100.0);
const float HAMMER_ANGLE = -45;
const float SICKLE_ANGLE = -45;

void main() {
    vec2 scaledUV = vec2(uv.x, 1.0 - uv.y) * vec2(800.0, 400.0);

    float hammer = hammer(scaledUV, SUMBOL_CENTER, HAMMER_ANGLE);
    float scytheShape = sickle(scaledUV, SUMBOL_CENTER, SICKLE_ANGLE);
    float star = starBorder(scaledUV, SUMBOL_CENTER + STAR_OFFSET, STAR_RADIUS, STAR_SIZE);

    float picture = max(max(hammer, scytheShape), star);

    vec3 finalColor = mix(RED_COLOR, YELLOW_COLOR, picture);

    FragColor = vec4(finalColor, 1.0f);
}