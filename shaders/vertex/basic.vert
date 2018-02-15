#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    vec4 world_position, view_position, homogenous_screenspace_position;
    vec3 world_eye_vec;
    vec3 out_normal, out_tangent, out_bitangent;
    TransformPosition(  in_position, world_position, view_position, homogenous_screenspace_position, world_eye_vec );
    world_normal = TransformNormal( in_normal );
    TransformNormal( in_tangent, out_tangent );
    TransformNormal( in_bitangent, out_bitangent );
    CreateVertexShaderOut( homogenous_screenspace_position, world_position, world_normal, world_eye_vec, in_color_uv );
}