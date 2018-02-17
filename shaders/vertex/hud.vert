#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    // compute position
    gl_Position = transform_position(  in_position, out_worldspace_position );
    
    // compute uvs
    out_color_uv = in_color_uv.xy;
}