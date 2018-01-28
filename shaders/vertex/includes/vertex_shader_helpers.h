void TransformPosition( in vec3 objectspace_position, out vec4 worldspace_position, out vec4 viewspace_position, out vec4 homogenous_screenspace_position, out vec3 worldposition_to_eye_position )
{
	worldspace_position				= WorldTransform * vec4(objectspace_position,1.0);
	viewspace_position				= ViewTransform * worldspace_position;
	homogenous_screenspace_position = ProjectionTransform * viewspace_position;
	worldposition_to_eye_position	= normalize(eye_position - worldspace_position.xyz);
}

void TransformNormal( in vec3 objectspace_normal, out vec3 worldspace_normal )
{
	worldspace_normal = normalize(WorldInverseTranspose * vec4(objectspace_normal,0)).xyz;
	//worldspace_normal = normalize(mul(objectspace_normal, WorldTransform));
	//worldspace_normal = normalize( vec4(objectspace_normal,0) * WorldInverseTranspose).xyz;
}

void CreateVertexShaderOut( vec4 position, vec4 world_position, vec3 normal, vec3 worldposition_to_eye_position, vec3 color_uv )
{
	gl_Position = position;
    out_worldspace_position = world_position;
	out_normal = normal;
	out_worldposition_to_eye_position = worldposition_to_eye_position;
	out_color_uv = color_uv.xy;
	//output.color_index = color_uv.z;
}