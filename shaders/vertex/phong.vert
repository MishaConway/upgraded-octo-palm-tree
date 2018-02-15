#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    vec4 world_position, view_position, homogenous_screenspace_position;
    vec3 world_eye_vec;
    
    TransformPosition(  in_position, out_worldspace_position, view_position, homogenous_screenspace_position, out_worldposition_to_eye_position );
    out_world_normal = TransformNormal( in_normal );
    out_world_tangent = TransformNormal( in_tangent );
    out_world_bitangent = TransformNormal( in_bitangent );
    
    out_color_uv = in_color_uv.xy;
    gl_Position = homogenous_screenspace_position;
}