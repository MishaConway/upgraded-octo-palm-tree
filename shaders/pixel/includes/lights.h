#define MAX_LIGHTS 10

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
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
uniform int num_lights;
uniform Material material;

float attenuation_factor( vec3 coefficients, float distance ){
    return 1.0 / (coefficients.x + coefficients.y * distance + coefficients.z * distance * distance );
}


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

vec3 apply_light(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.position.w == 0.0) {
        //directional light
        surfaceToLight = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = attenuation_factor( light.attenuation, distanceToLight );
        
        //cone restrictions (affects attenuation)
        /*float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }*/
    }
    
    //ambient
    vec3 ambient = light.ambient * material.ambient * surfaceColor;
    
    //diffuse
    float diffuse_coefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuse_coefficient * surfaceColor.rgb * light.diffuse * material.diffuse;
    
    //specular
    float specular_coefficient = 0.0;
    if(diffuse_coefficient > 0.0)
        specular_coefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess );
    vec3 specular = specular_coefficient * material.specular * light.specular;
    
    vec3 emissive = material.emissive * surfaceColor;
    
    //linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular) + emissive;
}



