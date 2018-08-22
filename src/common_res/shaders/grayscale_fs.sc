$input v_texcoord0

#include "bgfx_shader.sh"

SAMPLER2D(s_texture0, 0);

uniform vec4 u_progress;

void main()
{
    vec4 textureColor = texture2D(s_texture0, v_texcoord0);
    float progress = u_progress.x;

    vec3 weight = vec3(0.2125, 0.7154, 0.0721);
    float luminance = dot(textureColor.rgb, weight);
    vec3 luminanceColor = vec3(luminance, luminance, luminance);
    vec3 mixColor = mix(textureColor.rgb, luminanceColor, progress);
    vec4 fragColor = vec4(mixColor, textureColor.a);

    gl_FragColor = fragColor;
}
