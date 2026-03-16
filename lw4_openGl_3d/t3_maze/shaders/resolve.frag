#version 460 core
#extension GL_ARB_shader_image_load_store : require

in vec2 vTexCoord;
out vec4 FragColor;

struct OITFragment
{
    vec4 color;
    float depth;
    uint next;
};

layout(binding = 0, r32ui) uniform uimage2D uHeadPointers;
layout(std430, binding = 2) buffer FragmentBuffer
{
    OITFragment fragments[];
} fragBuffer;

uniform sampler2D uBackground; 
uniform sampler2D uBackgroundDepth;

const int MAX_LAYERS = 16;

void main()
{
    ivec2 screenPos = ivec2(gl_FragCoord.xy);
    
    vec4 background = texture(uBackground, vTexCoord);
    float backgroundDepth = texture(uBackgroundDepth, vTexCoord).r;
    
    uint head = imageLoad(uHeadPointers, screenPos).r;
    
    if (head == 0) {
        FragColor = background;
        return;
    }

    vec4 colors[MAX_LAYERS+1];
    float depths[MAX_LAYERS+1];
    int count = 0;
    
    uint current = head;
    while (current != 0) {
        OITFragment frag = fragBuffer.fragments[current];
        current = frag.next;
        if (frag.depth > backgroundDepth) {
            continue;
        }

        int pos = count;
        
        while (pos > 0 && depths[pos - 1] > frag.depth) {
            depths[pos] = depths[pos-1];
            colors[pos] = colors[pos-1];
            --pos;
        }
        
        depths[pos] = frag.depth;
        colors[pos] = frag.color;
        
        if (count < MAX_LAYERS) {
            ++count;
        }
    }
    
    vec4 result = background;
    for (int i = count - 1; i >= 0; --i) {
        result.rgb = mix(result.rgb, colors[i].rgb, colors[i].a);
    }
    
    FragColor = result;
}