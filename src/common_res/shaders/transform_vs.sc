$input a_position, a_texcoord0
$output v_texcoord0

uniform mat4 u_transform;

#include "bgfx_shader.sh"

void main()
{
    vec4 position = vec4(a_position, 0.0, 1.0);
    position = mul(u_transform, position);
    gl_Position = position;
    v_texcoord0 = a_texcoord0;
}
