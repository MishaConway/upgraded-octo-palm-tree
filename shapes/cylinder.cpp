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
        const float slice_uv_height = 1.0f / num_slices;
        
        const float bottom_y = -height / 2.0f + slice_height * slice;
        const float top_y = -height / 2.0f + slice_height * (slice+1);
        
        const float bottom_uv = slice * slice_uv_height;
        const float top_uv = bottom_uv + slice_uv_height;
        
        
        
        Vertex a( GeoFloat3( half_width * cos(left_angle),
                            bottom_y,
                            half_length * sin(left_angle) ),
                 GeoFloat2( left_angle / GEO_2PI, bottom_uv ));
        
        Vertex b( GeoFloat3( half_width * cos(right_angle),
                            bottom_y,
                            half_length * sin(right_angle) ),
                 GeoFloat2( right_angle / GEO_2PI, bottom_uv ) );
        
        Vertex c( GeoFloat3( half_width * cos(right_angle),
                            top_y,
                            half_length * sin(right_angle) ),
                 GeoFloat2( right_angle / GEO_2PI, top_uv ) );
        
        Vertex d( GeoFloat3( half_width * cos(left_angle),
                            top_y,
                            half_length * sin(left_angle) ),
                 GeoFloat2( left_angle / GEO_2PI, top_uv ) );
        
        a.normal = GeoVector( a.position.x, 0.0f, a.position.z ).Normalize().ToGeoFloat3();
        a.tangent = GeoVector( a.normal ).Cross( GeoVector( 0,1,0,1) ).ToGeoFloat4();
        a.bitangent = GeoFloat3( 0, 1, 0 );
        
        b.normal = GeoVector( b.position.x, 0.0f, b.position.z ).Normalize().ToGeoFloat3();
        b.tangent = GeoVector( b.normal ).Cross( GeoVector( 0,1,0,1) ).ToGeoFloat4();
        b.bitangent = GeoFloat3( 0, 1, 0 );

        c.normal = GeoVector( c.position.x, 0.0f, c.position.z ).Normalize().ToGeoFloat3();
        c.tangent = GeoVector( c.normal ).Cross( GeoVector( 0,1,0,1) ).ToGeoFloat4();
        c.bitangent = GeoFloat3( 0, 1, 0 );
        
        d.normal = GeoVector( d.position.x, 0.0f, d.position.z ).Normalize().ToGeoFloat3();
        d.tangent = GeoVector( d.normal ).Cross( GeoVector( 0,1,0,1) ).ToGeoFloat4();
        d.bitangent = GeoFloat3( 0, 1, 0 );


        

        
        
        quads.push_back( Quad( d, c, b, a ) );
    }
    
   // ReverseWinding();
}



