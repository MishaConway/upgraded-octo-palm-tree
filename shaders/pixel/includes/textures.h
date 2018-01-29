uniform sampler2D tex1;
uniform vec2 tex1_scale;
uniform vec2 tex1_dimensions;


vec4 SampleTex1( vec2 uv )
{
	return texture2D( tex1, vec2( uv.x * tex1_scale.x, uv.y * tex1_scale.y ) );
}

