#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"
#include "includes/lights.h"

// https://github.com/stackgl/glsl-lighting-walkthrough

void main()
{
    vec2 uv = out_color_uv.xy;
    

    vec3 tex1_sample = sample_tex_1( uv ).rgb;
    vec3 tangent_normal = sample_normal( sample_tex_2( uv ) );
    //tangent_normal = vec3( 0.0, 0.0, 1.0);
    

    vec3 world_normal = normalize(out_world_normal);
    vec3 world_tangent = normalize(out_world_tangent.xyz);
    //vec3 world_bitangent =  cross(world_normal, world_tangent) * out_world_tangent.w;
    vec3 world_bitangent =  normalize(out_world_bitangent);

    vec3 world_position = out_worldspace_position.xyz;
    vec3 world_position_to_eye_position = normalize(out_worldposition_to_eye_position);
    
    if( fudge == 1 ){
        world_normal = transform_basis( tangent_normal, world_tangent, world_bitangent, world_normal );
    }
    
    
    
    vec3 linear_color = vec3(0);
    for(int i = 0; i < num_lights; ++i){
        linear_color += apply_light(lights[i], tex1_sample, world_normal, world_position, world_position_to_eye_position);
    }
    linear_color = saturate( linear_color );
    linear_color = gamma_correct( linear_color );
    
    //gl_FragColor = vec4( out_color_uv.x, out_color_uv.x, out_color_uv.x, 1);
    
    //gl_FragColor = vec4( abs(world_normal.x), abs(world_normal.x), abs(world_normal.x)  , 1 );
    gl_FragColor = vec4( linear_color, 1.0 );
    
    if( fudge2 == 1 )
        world_normal *= -1.0;
    
    //gl_FragColor = vec4( world_normal, 1.0 );
}