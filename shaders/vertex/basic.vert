#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    vec4 worldPosition, viewPosition, homogenous_screenspace_position;
    vec3 worldNormal, worldEyeVec;
    float clip;
    TransformPosition(  in_position, worldPosition, viewPosition, homogenous_screenspace_position, worldEyeVec, clip );
    TransformNormal( in_normal, worldNormal );
    CreateVertexShaderOut( homogenous_screenspace_position, worldPosition, worldNormal, worldEyeVec, in_color_uv, clip );
}