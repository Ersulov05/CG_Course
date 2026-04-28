#version 460 core
layout (lines) in;
layout (line_strip, max_vertices = 6) out;

in vec4 vertexColor[];
out vec4 geometryColor;

const float ARROW_ANGLE_RAD = 30.0 * 3.1415926 / 180.0;
const float ARROW_SIZE_COEF = 0.2;

void drawLine(vec4 p1, vec4 p2, vec4 color)
{
    geometryColor = color;  
    gl_Position = p1;
    EmitVertex();

    geometryColor = color;  
    gl_Position = p2;
    EmitVertex();
    EndPrimitive();
}

vec2 rotate(vec2 dir, float radAngle)
{
    return vec2(
        dir.x * cos(radAngle) - dir.y * sin(radAngle),
        dir.x * sin(radAngle) + dir.y * cos(radAngle)
    );
}

void main() {
    vec4 P1 = gl_in[0].gl_Position;
    vec4 P2 = gl_in[1].gl_Position;  

    vec2 dir = vec2(P1.x - P2.x, P1.y - P2.y);
    float arrowSize = length(dir) * ARROW_SIZE_COEF;
    dir = normalize(dir);

    vec2 leftDir = rotate(dir, -ARROW_ANGLE_RAD);
    vec2 rightDir = rotate(dir, ARROW_ANGLE_RAD);

    vec2 tip = vec2(P2.x, P2.y);
    vec4 leftEnd = vec4(tip + leftDir * arrowSize, P2.z, P2.w);
    vec4 rightEnd = vec4(tip + rightDir * arrowSize, P2.z, P2.w);

    drawLine(P1, P2, vertexColor[1]);
    drawLine(P2, leftEnd, vertexColor[1]);
    drawLine(P2, rightEnd, vertexColor[1]);
}