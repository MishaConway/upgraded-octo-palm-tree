#include "../includes/uniforms.h"
#include "includes/attributes.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/vertex_shader_helpers.h"


void main()
{
    vec4 worldPosition, viewPosition, homogenous_screenspace_position;
    vec3 worldNormal, worldEyeVec;
    TransformPosition(  in_position, worldPosition, viewPosition, homogenous_screenspace_position, worldEyeVec );
    TransformNormal( in_normal, worldNormal );
    CreateVertexShaderOut( homogenous_screenspace_position, worldPosition, worldNormal, worldEyeVec, in_color_uv );
}