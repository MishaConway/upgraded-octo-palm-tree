#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"

// https://github.com/stackgl/glsl-lighting-walkthrough

void main(void)
{
    vec3 world_normal = normalize(out_world_normal);
    vec3 worldposition_to_eye_position = normalize(out_worldposition_to_eye_position);
    
    
    vec3 light_dir = normalize( vec3( 0, 1, 1) );
    
    float diffuse_factor = dot( light_dir, world_normal );
    
    vec3 incident_reflection = reflect( light_dir, world_normal);
    
    
    
    float cosAngle = max(0.0, dot(-worldposition_to_eye_position, incident_reflection));
    float specularCoefficient = pow(cosAngle, 64.0);
    
    
    vec3 tex1_sample = SampleTex1(out_color_uv.xy).rgb;
    
    vec3 diffuse_color = diffuse_factor * tex1_sample;
    vec3 specular_color = specularCoefficient * vec3(1,1,1);
    
    
    vec4 linearColor =  vec4( saturate(diffuse_color + specular_color), 1 );
    
    gl_FragColor = linearColor;
    
    //gl_FragColor = vec4( tex1_sample, 1 );

    //gl_FragColor = vec4( out_color_uv.y, out_color_uv.y, out_color_uv.y, 1 );//linearColor;
    
    //gl_FragColor = vec4( world_normal, 1);
    
    //gl_FragColor = vec4( world_normal, 1.0);

    //gl_FragColor = gamma_correct( linearColor );
}