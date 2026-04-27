#version 460 core
#extension GL_ARB_shader_image_load_store : require
#extension GL_ARB_shader_atomic_counters : require

uniform vec3 uLightPos;

in vec3 worldPos;
in vec3 worldNormal;
in vec4 vertexColor;
out vec4 FragColor;

struct OITFragment {
    vec4 color;
    float depth;
    uint next;
};

layout(binding = 0, r32ui) coherent uniform uimage2D uHeadPointers;
layout(binding = 1, offset = 0) uniform atomic_uint uFragmentCounter;
layout(std430, binding = 2) coherent buffer FragmentBuffer {
    OITFragment fragments[];
} fragBuffer;

const uint MAX_FRAGMENTS = 4 * 1024 * 1024;

vec4 calculateLighting(vec3 pos, vec3 normal, vec4 albedo) {
    vec3 N = normalize(normal);
    vec3 lightDir = uLightPos - pos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    
    float ambient = 0.2;
    float diffuse = abs(dot(N, lightDir));
    
    float attenuation = 1.0 / (distance);
    
    float brightness = ambient + diffuse * attenuation;
    
    brightness = min(brightness, 1.0);
    
    return vec4(albedo.rgb * brightness, albedo.a);
}

void main() {
    vec4 litColor = calculateLighting(worldPos, worldNormal, vertexColor);

    if (vertexColor.a >= 0.999) {
        FragColor = litColor;
        return;
    }
    
    ivec2 screenPos = ivec2(gl_FragCoord.xy);
    
    uint newIndex = atomicCounterIncrement(uFragmentCounter);
    
    if (newIndex >= MAX_FRAGMENTS) {
        discard;
        return;
    }
    
    uint oldHead = imageAtomicExchange(uHeadPointers, screenPos, newIndex);
    
    fragBuffer.fragments[newIndex].color = litColor;
    fragBuffer.fragments[newIndex].depth = gl_FragCoord.z;
    fragBuffer.fragments[newIndex].next = oldHead;
    
    discard;
}