vec4 transform_position( in vec3 objectspace_position, out vec4 worldspace_position )
{
	worldspace_position				= world_transform * vec4(objectspace_position,1.0);
	vec4 viewspace_position			= view_transform * worldspace_position;
	vec4 homogenous_screenspace_position = projection_transform * viewspace_position;
    return homogenous_screenspace_position;
}

vec3 transform_normal( vec3 normal ){
    return normalize(world_inverse_transpose * vec4(normal,0)).xyz;
}

vec4 transform_normal( vec4 normal ){
    return vec4( transform_normal(normal.xyz), normal.w );
}

