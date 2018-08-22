$input v_texcoord0

#include "bgfx_shader.sh"

SAMPLER2D(s_texture0, 0);
SAMPLER2D(s_texture1, 1);

uniform vec4 u_mixUniform;

void main()
{
    vec4 texColor0 = texture2D(s_texture0, v_texcoord0);
    vec4 texColor1 = texture2D(s_texture1, v_texcoord0);
    float mixValue = u_mixUniform.x;
    vec4 result = mix(texColor0, texColor1, mixValue);
	gl_FragColor = result;
}
