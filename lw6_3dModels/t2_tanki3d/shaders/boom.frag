#version 460 core

out vec4 FragColor;

uniform float uTime;  // 0..1

void main() {
    
    float alpha = 1.0 - uTime;
    vec3 color = mix(vec3(1.0, 0.5, 0.2), vec3(0.2, 0.05, 0.0), uTime);
    
    FragColor = vec4(color, alpha);
}