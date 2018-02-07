float saturate( float val )
{
    return clamp(val, 0.0, 1.0);
}

vec3 saturate( vec3 val )
{
    return clamp(val, 0.0, 1.0);
}

vec4 saturate( vec4 val ){
    return clamp( val, 0.0, 1.0 );
}

vec3 gamma_correct( vec3 val ){
    vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
    return vec3(pow(val.r, gamma.r),
                pow(val.g, gamma.g),
                pow(val.b, gamma.b));
}

vec4 gamma_correct( vec4 val ){
    vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
    return vec4(pow(val.r, gamma.r),
                pow(val.g, gamma.g),
                pow(val.b, gamma.b),val.a);
}

float gamma_to_linear(float v) {
    return pow(v, 2.2);
}

vec2 gamma_to_linear(vec2 v) {
    return pow(v, vec2(2.2));
}

vec3 gamma_to_linear(vec3 v) {
    return pow(v, vec3(2.2));
}

vec4 gamma_to_linear(vec4 v) {
    return vec4(gamma_to_linear(v.rgb), v.a);
}