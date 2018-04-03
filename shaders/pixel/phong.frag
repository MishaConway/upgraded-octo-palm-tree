#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"
#include "includes/toon.h"
#include "includes/lights.h"

// https://github.com/stackgl/glsl-lighting-walkthrough

// http://roxlu.com/2014/037/opengl-rim-shader

// https://blog.felixkate.net/2017/01/19/toon-shading/

void main()
{
    vec2 uv = out_color_uv.xy;
    vec3 tex1_sample = sample_tex_1( uv ).rgb;
    vec3 tangent_normal = sample_normal( sample_tex_2( uv ) );
    //tangent_normal = vec3(0,0,1);
    
    
    vec3 world_normal = normalize(out_world_normal);
    vec3 world_vertex_normal = world_normal;
    vec3 world_tangent = normalize(out_world_tangent.xyz);
    vec3 world_bitangent =  normalize(out_world_bitangent);

    vec3 world_position = out_worldspace_position.xyz;
    vec3 world_position_to_eye_position = normalize(out_worldposition_to_eye_position);
    
    

    
    if( fudge == 1 ){
        world_normal = transform_basis2( tangent_normal, world_tangent, world_bitangent, world_normal );
    }
    
    vec3 linear_color = vec3(0);
    for(int i = 0; i < num_lights; ++i){
        linear_color += apply_light(lights[i], tex1_sample, world_normal, world_vertex_normal, world_position, world_position_to_eye_position);
    }
    
    //linear_color = linear_color );
    
    //linear_color = world_normal;
    //linear_color = world_bitangent;
    //linear_color = vec3( out_world_tangent.w, out_world_tangent.w , out_world_tangent.w  );
    
    
    
    gl_FragColor = vec4( linear_color, 1.0 );
}