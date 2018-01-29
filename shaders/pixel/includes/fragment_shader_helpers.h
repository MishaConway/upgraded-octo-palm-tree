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

vec4 gamma_correct( vec3 val ){
    vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
    return vec4(pow(val.r, gamma.r),
                pow(val.g, gamma.g),
                pow(val.b, gamma.b),1.0);
}

vec4 gamma_correct( vec4 val ){
    vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
    return vec4(pow(val.r, gamma.r),
                pow(val.g, gamma.g),
                pow(val.b, gamma.b),val.a);
}