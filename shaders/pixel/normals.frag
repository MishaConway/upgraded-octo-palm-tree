#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"
#include "includes/toon.h"
#include "includes/lights.h"


void main()
{
    vec2 uv = out_color_uv.xy;
    vec3 tangent_normal = sample_normal( sample_tex_2( uv ) );
    
    vec3 world_normal = normalize(out_world_normal);
    vec3 world_tangent = normalize(out_world_tangent.xyz);
    vec3 world_bitangent =  normalize(out_world_bitangent);
    
    if( fudge == 1 ){
        world_normal = transform_basis( tangent_normal, world_tangent, world_bitangent, world_normal );
    }
    
    gl_FragColor = vec4( world_normal, 1.0 );
}