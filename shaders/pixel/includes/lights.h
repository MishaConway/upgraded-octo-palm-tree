#define MAX_LIGHTS 10

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform struct Light {
    vec4 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    
    vec3 attenuation;
    
    float cone_angle;
};

uniform Light lights[MAX_LIGHTS];
uniform Material material;


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



