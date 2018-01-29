#include "../includes/uniforms.h"
#include "../includes/varying.h"
#include "../includes/helpers.h"
#include "includes/textures.h"

void main(void)
{
    vec3 light_dir = normalize( vec3( 0, 1, 1) );
    
    float diffuse_factor = dot( light_dir, out_world_normal );
    
    
    gl_FragColor =  vec4( diffuse_factor * vec3(1,1,1), 1 );
    
    //gl_FragColor = vec4( out_world_normal, 1 );
}