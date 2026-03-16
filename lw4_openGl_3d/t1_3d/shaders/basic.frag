#version 460 core
#extension GL_ARB_shader_image_load_store : require
#extension GL_ARB_shader_atomic_counters : require

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

void main() {
    if (vertexColor.a >= 0.999) {
        FragColor = vertexColor;
        return;
    }
    
    ivec2 screenPos = ivec2(gl_FragCoord.xy);
    
    uint newIndex = atomicCounterIncrement(uFragmentCounter);
    
    if (newIndex >= MAX_FRAGMENTS) {
        discard;
        return;
    }
    
    uint oldHead = imageAtomicExchange(uHeadPointers, screenPos, newIndex);
    
    fragBuffer.fragments[newIndex].color = vertexColor;
    fragBuffer.fragments[newIndex].depth = gl_FragCoord.z;
    fragBuffer.fragments[newIndex].next = oldHead;
    
    discard;
}