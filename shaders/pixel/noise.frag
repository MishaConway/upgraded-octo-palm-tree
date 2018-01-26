#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float rand(in vec2 _xy)	{
	return fract(
		sin(
			dot(
				_xy.xy,
				vec2(123.3, 47.22)
			)
		)*82231.29
	);
}

float noise(in vec2 _xy)	{
	// Variables
	vec2	i=	floor(_xy);
	vec2	f=	fract(_xy);
	float	a=	rand(i);
	float	b=	rand(i+vec2(1.0, 0.0));
	float	c=	rand(i+vec2(0.0, 1.0));
	float	d=	rand(i+vec2(1.0, 1.0));
	vec2	u=	smoothstep(0.0, 1.0, f);
	
	return (
		mix(a, b, u.x)+
		(c-a)*u.y*(1.0-u.x)+
		(d-b)*u.x*u.y
	);
}

void main( void ) {
	// Variables
	vec2	uv=	gl_FragCoord.xy;
	float	n=	noise(uv*0.5);
	
	gl_FragColor=	vec4(vec3(n), 1.0);
}
