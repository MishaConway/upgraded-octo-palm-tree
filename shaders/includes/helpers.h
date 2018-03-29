vec2 Scale( vec2 vec, float horizontal_scale, float vertical_scale )
{
	return vec2( vec.x * horizontal_scale, vec.y * vertical_scale );
}

vec2 InvertY( vec2 vec )
{
	vec.y *= -1.0;
	return vec;
}

vec3 InvertY( vec3 vec )
{
	vec.y *= -1.0;
	return vec;
}

vec4 InvertY( vec4 vec )
{
	vec.y *= -1.0;
	return vec;
}

vec3 transform_basis( vec3 vec, vec3 x_axis, vec3 y_axis, vec3 z_axis ){
    return normalize(vec3(
        dot( vec, x_axis),
        dot( vec, y_axis),
        dot( vec, z_axis)
    ));
}

vec3 transform_basis2( vec3 vec, vec3 x_axis, vec3 y_axis, vec3 z_axis ){
    mat3 tbn = mat3( x_axis, y_axis, z_axis );
    return tbn * vec;
}

