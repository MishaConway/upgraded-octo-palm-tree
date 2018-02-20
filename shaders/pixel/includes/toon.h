





float toonify_factor( float factor, float levels ){
    const float scale_factor = 1.0 / levels;
    
    float num_levels = floor(factor / scale_factor);
    
    return num_levels * factor;
    
    
    //return floor(diffuse_factor * levels) * scale_factor;
}