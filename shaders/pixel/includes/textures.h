uniform sampler2D tex1;
uniform vec2 tex1_scale;
uniform vec2 tex1_dimensions;

uniform sampler2D tex2;
uniform vec2 tex2_scale;
uniform vec2 tex2_dimensions;


vec4 sample_tex_1( vec2 uv )
{
	return texture2D( tex1, vec2( uv.x * tex1_scale.x, uv.y * tex1_scale.y ) );
}

vec4 sample_tex_2( vec2 uv )
{
    return texture2D( tex2, vec2( uv.x * tex2_scale.x, uv.y * tex2_scale.y ) );
}

vec3 sample_normal( vec4 color ){
    return (color.xyz-0.5)*2.0;
}
 


