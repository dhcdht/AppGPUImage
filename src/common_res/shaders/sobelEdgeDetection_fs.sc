$input v_texcoord0

#include "bgfx_shader.sh"

SAMPLER2D(s_texture0, 0);

uniform vec4 u_pixelUniform;

static const vec3 kWeight = vec3(0.2125, 0.7154, 0.0721);
float getLuminance(vec2 coord) 
{
    vec3 textureColor = texture2D(s_texture0, coord).rgb;
    float luminance = dot(textureColor.rgb, kWeight);

    return luminance;
}

void main()
{
    float pixelWidth = u_pixelUniform.x;
    float pixelHeight = u_pixelUniform.y;
    float edgeStrength = u_pixelUniform.z;

    vec2 texSize = textureSize(s_texture0, 0);
    float texelWidth = pixelWidth / texSize.x;
    float texelHeight = pixelHeight / texSize.y;

    vec2 widthStep = vec2(texelWidth, 0.0);
    vec2 heightStep = vec2(0.0, texelWidth);

    vec2 tc = v_texcoord0.xy;
    vec2 leftTC = tc - widthStep;
    vec2 rightTC = tc + widthStep;

    vec2 topTC = tc - heightStep;
    vec2 topLeftTC = topTC - widthStep;
    vec2 topRightTC = topTC + widthStep;

    vec2 bottomTC = tc + heightStep;
    vec2 bottomLeftTC = bottomTC - widthStep;
    vec2 bottomRightTC = bottomTC + widthStep;

    float intensity = getLuminance(tc);
    float leftIntensity = getLuminance(leftTC);
    float rightIntensity = getLuminance(rightTC);

    float topIntensity = getLuminance(topTC);
    float topLeftIntensity = getLuminance(topLeftTC);
    float topRightIntensity = getLuminance(topRightTC);

    float bottomIntensity = getLuminance(bottomTC);
    float bottomLeftIntensity = getLuminance(bottomLeftTC);
    float bottomRightIntensiry = getLuminance(bottomRightTC);

    float h = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensiry;
    float v = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensiry + 2.0 * rightIntensity + topRightIntensity;

    float mag = length(vec2(h, v)) * edgeStrength;

	gl_FragColor = vec4(mag, mag, mag, 1.0);
}
