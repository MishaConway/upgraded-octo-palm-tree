#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    vec4 world_position, view_position, homogenous_screenspace_position;
    vec3 world_normal, world_eye_vec;
    TransformPosition(  in_position, world_position, view_position, homogenous_screenspace_position, world_eye_vec );
    TransformNormal( in_normal, world_normal );
    CreateVertexShaderOut( homogenous_screenspace_position, world_position, world_normal, world_eye_vec, in_color_uv );
}