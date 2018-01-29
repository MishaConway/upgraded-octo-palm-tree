#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/fragment_shader_helpers.h"
#include "includes/textures.h"

void main(void)
{
    vec3 world_normal = normalize(out_world_normal);
    vec3 worldposition_to_eye_position = normalize(out_worldposition_to_eye_position);
    
    
    vec3 light_dir = normalize( vec3( 0, 1, 1) );
    
    float diffuse_factor = dot( light_dir, world_normal );
    
    vec3 incident_reflection = reflect( light_dir, world_normal);
    
    
    
    float cosAngle = max(0.0, dot(-worldposition_to_eye_position, incident_reflection));
    float specularCoefficient = pow(cosAngle, 64.0);
    
    
    vec4 linearColor =  vec4( saturate((diffuse_factor + specularCoefficient) * vec3(1,1,1)), 1 );

    gl_FragColor = linearColor;

    //gl_FragColor = gamma_correct( linearColor );
}