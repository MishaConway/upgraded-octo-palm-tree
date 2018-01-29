#include "cylinder.h"

Cylinder::~Cylinder(){
    

}


Cylinder::Cylinder( const float width, const float height, const float length ){
    const unsigned int num_wedges = 260;
    
    const float half_width = width / 2.0f;
    const float half_length = length / 2.0f;
    
    for( unsigned int i = 0; i < num_wedges; i++ )
    {
        const float left_angle = GeoConvertToRadians( i * 360.0f / (float) num_wedges );
        const float right_angle = GeoConvertToRadians( (i+1) * 360.0f / (float) num_wedges );
        
        Vertex a( GeoFloat3( half_width * cos(left_angle),
                            -height / 2.0f,
                            half_length * sin(left_angle) ),
                 GeoFloat2( 50*left_angle / GEO_2PI, 0 ) );
        
        Vertex b( GeoFloat3( half_width * cos(right_angle),
                            -height / 2.0f,
                            half_length * sin(right_angle) ),
                 GeoFloat2( 50*right_angle / GEO_2PI, 0 ) );
        
        Vertex c( GeoFloat3( half_width * cos(right_angle),
                            height / 2.0f,
                            half_length * sin(right_angle) ),
                 GeoFloat2( 50*right_angle / GEO_2PI, 1 ) );
        
        Vertex d( GeoFloat3( half_width * cos(left_angle),
                            height / 2.0f,
                            half_length * sin(left_angle) ),
                 GeoFloat2( 50*left_angle / GEO_2PI, 1 ) );
        
        quads.push_back( Quad( d, c, b, a ) );
    }
    
   // ReverseWinding();
}



