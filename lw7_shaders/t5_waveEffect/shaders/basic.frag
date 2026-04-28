#version 460 core

uniform sampler2D uTextures[2];
uniform int uTextureCount;
uniform float uTime;
uniform vec2 uClickUV;

in vec2 uv;
in vec4 vertexColor;

out vec4 FragColor;

const float M_PI = 3.1415926;
const float SQRT_OF_TWO = 1.4142;
const float WAVE_COEF = 0.7;
const float WAVE_INTENSITY_COEF = 0.65;

float waveEffect(vec2 uv, vec2 center, float progress, out float waveIntensity) 
{
    float dist = distance(uv, center);
    
    float waveRadius = progress * SQRT_OF_TWO;
    float waveWidth = sqrt(progress) * WAVE_COEF;

    float mixFactor;
    if (dist < waveRadius) {
        mixFactor = 0.0;
        waveIntensity = 1.0;
    } else if (dist > waveRadius + waveWidth) {
        mixFactor = 1.0;
        waveIntensity = 1.0;
    } else {
        float factor = (dist - waveRadius) / waveWidth;
        mixFactor = min(factor, 1.0);
        waveIntensity = 1.0 - sin(factor * M_PI) * WAVE_INTENSITY_COEF;
    }
        
    return mixFactor;
}

float getFactor(float t)
{
    return pow(t, 2.0);
}

void main() {
    vec4 color1 = vertexColor;
    vec4 color2 = vertexColor;
    if (uTextureCount > 0) {
        color1 = texture(uTextures[0], uv);

        if (uTextureCount > 1) {
            color2 = texture(uTextures[1], uv);
        }
    }
    
    float timeFactor = getFactor(clamp(uTime, 0.0, 1.0));

    float waveIntensity;
    float mixFactor = waveEffect(uv, uClickUV, timeFactor, waveIntensity);

    vec3 finalColor = mix(color1.rgb, color2.rgb, mixFactor);
    finalColor *= waveIntensity;

    FragColor = vec4(finalColor, 1.0);
}