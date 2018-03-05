
float stepmix(float edge0, float edge1, float E, float x)
{
    float T = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
    return mix(edge0, edge1, T);
}




float toonify_factor( float factor, float levels ){
    float E = fwidth(factor);
    
    

    
    float amount_per_level = 1.0 / levels;
    
    float num_levels = floor(factor / amount_per_level);
    
    return num_levels * amount_per_level;
}

vec3 ramp( float f ){
    return sample_tex_3( vec2( f, f) ).rgb;
    
    
}