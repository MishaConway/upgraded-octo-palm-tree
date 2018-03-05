#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    // compute position
    gl_Position = vec4(  in_position, 1.0 );
    
    // compute uvs
    out_color_uv = in_color_uv.xy;
}