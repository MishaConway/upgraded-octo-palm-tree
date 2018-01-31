#include "cylinder.h"

Cylinder::~Cylinder(){
    

}


Cylinder::Cylinder( const float width, const float height, const float length ){
    const unsigned int num_wedges = 64;
    const unsigned int num_slices = 32;
    
    const float half_width = width / 2.0f;
    const float half_length = length / 2.0f;
    
    for( unsigned int slice = 0; slice < num_slices; slice++ )
    for( unsigned int i = 0; i < num_wedges; i++ )
    {
        const float left_angle = GeoConvertToRadians( i * 360.0f / (float) num_wedges );
        const float right_angle = GeoConvertToRadians( (i+1) * 360.0f / (float) num_wedges );
        
        const float slice_height = height / num_slices;
        
        const float bottom_y = -height / 2.0f + slice_height * slice;
        const float top_y = -height / 2.0f + slice_height * (slice+1);
        
        
        Vertex a( GeoFloat3( half_width * cos(left_angle),
                            bottom_y,
                            half_length * sin(left_angle) ),
                 GeoFloat2( 50*left_angle / GEO_2PI, 0 ));
        
        Vertex b( GeoFloat3( half_width * cos(right_angle),
                            bottom_y,
                            half_length * sin(right_angle) ),
                 GeoFloat2( 50*right_angle / GEO_2PI, 0 ) );
        
        Vertex c( GeoFloat3( half_width * cos(right_angle),
                            top_y,
                            half_length * sin(right_angle) ),
                 GeoFloat2( 50*right_angle / GEO_2PI, 1 ) );
        
        Vertex d( GeoFloat3( half_width * cos(left_angle),
                            top_y,
                            half_length * sin(left_angle) ),
                 GeoFloat2( 50*left_angle / GEO_2PI, 1 ) );
        
        
        a.normal = GeoVector( a.position.x, 0.0f, a.position.z ).Normalize().ToGeoFloat3();
        b.normal = GeoVector( b.position.x, 0.0f, b.position.z ).Normalize().ToGeoFloat3();
        c.normal = GeoVector( c.position.x, 0.0f, c.position.z ).Normalize().ToGeoFloat3();
        d.normal = GeoVector( d.position.x, 0.0f, d.position.z ).Normalize().ToGeoFloat3();

        

        
        
        quads.push_back( Quad( d, c, b, a ) );
    }
    
   // ReverseWinding();
}



