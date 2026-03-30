#version 460 core

uniform sampler2D uTextures[2];
uniform int uTextureCount;
uniform vec3 uLightPos;

in vec3 worldPos;
in vec3 worldNormal;
in vec4 vertexColor;
in vec2 texCoord;

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

vec4 calculateLighting(vec3 pos, vec3 normal, vec4 albedo, bool isAlpha) {
    vec3 N = normalize(normal);
    vec3 lightDir = uLightPos - pos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    
    float ambient = 0.2;
    float diffuse = isAlpha
        ? abs(dot(N, lightDir))
        : max(dot(N, lightDir), 0.0); 
    
    float attenuation = 1.0 / (distance + 0.1);
    
    float brightness = ambient + diffuse * attenuation * 4;
    
    brightness = min(brightness, 1.0);
    
    return vec4(albedo.rgb * brightness, albedo.a);
}

void main() {
    vec4 albedo;
    if (uTextureCount > 0) {
        albedo = texture(uTextures[0], texCoord);

        if (uTextureCount > 1) {
            vec4 secondTex = texture(uTextures[1], texCoord);
            
            float shadowMask = secondTex.r;
            //albedo.rgb *= shadowMask;
            //albedo = vec4(1.0, 1.0, 1.0, 1.0);
            albedo.rgb *= shadowMask;

            //albedo = texture(uTextures[1], texCoord);
        }
    } else {
        albedo = vertexColor;
    }
    

    if (albedo.a >= 0.999) {
        FragColor = calculateLighting(worldPos, worldNormal, albedo, false);

        return;
    }
    vec4 litColor = calculateLighting(worldPos, worldNormal, albedo, true);
    
    uint newIndex = atomicCounterIncrement(uFragmentCounter);
    
    if (newIndex >= MAX_FRAGMENTS) {
        discard;
        return;
    }

    ivec2 screenPos = ivec2(gl_FragCoord.xy);
    uint oldHead = imageAtomicExchange(uHeadPointers, screenPos, newIndex);
    
    fragBuffer.fragments[newIndex].color = litColor;
    fragBuffer.fragments[newIndex].depth = gl_FragCoord.z;
    fragBuffer.fragments[newIndex].next = oldHead;
    
    discard;
}