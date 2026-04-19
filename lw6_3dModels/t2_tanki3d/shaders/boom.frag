#version 460 core

out vec4 FragColor;

uniform float uTime;  // 0..1

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

void main() {
    
    float alpha = 1.0 - uTime;
    vec3 color = mix(vec3(1.0, 0.5, 0.2), vec3(0.2, 0.05, 0.0), uTime);
    
    if (alpha >= 0.999) {
        FragColor = vec4(color, alpha);
        return;
    }
    
    uint newIndex = atomicCounterIncrement(uFragmentCounter);
    if (newIndex >= MAX_FRAGMENTS) {
        discard;
    }

    ivec2 screenPos = ivec2(gl_FragCoord.xy);
    uint oldHead = imageAtomicExchange(uHeadPointers, screenPos, newIndex);
    
    fragBuffer.fragments[newIndex].color = vec4(color, alpha);
    fragBuffer.fragments[newIndex].depth = gl_FragCoord.z;
    fragBuffer.fragments[newIndex].next = oldHead;

    discard;
}