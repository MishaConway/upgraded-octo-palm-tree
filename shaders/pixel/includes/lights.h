#define MAX_LIGHTS 10

uniform struct Light {
    vec4 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    
    vec3 attenuation;
    
    float cone_angle;
    vec3 cone_direction;
};

uniform Light lights[MAX_LIGHTS];


vec3 light_direction( vec4 light, vec3 position ){
    if( 0.0 == light.w )
        return light.xyz;
    return normalize(light.xyz - position );
}

vec3 light_direction( Light light, vec3 position ){
    return light_direction( light.position, position );
}

vec3 light_direction( Light light, vec4 position ){
    return light_direction( light, position.xyz );
}



