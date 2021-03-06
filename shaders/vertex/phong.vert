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

    // compute normalish vectors
    out_world_normal = transform_normal( in_normal );
    out_world_tangent = transform_normal( in_tangent );
    out_world_bitangent = transform_normal( in_bitangent );
    
    // compute eye vectors
    out_worldposition_to_eye_position =	normalize(eye_position - out_worldspace_position.xyz);
}