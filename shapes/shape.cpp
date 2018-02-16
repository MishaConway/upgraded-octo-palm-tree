#include "shape.h"
#include "shape_utilities/tangents.h"
#include "shape_utilities/tgen.h"











Shape::~Shape(){
    
}

std::vector<Vertex> Shape::ToVertices(){
    auto verts = ToVerticesEx();
    //CalculateTangentArray( verts );
    
 
    
    
    
    
    return verts;
}

Shape& Shape::Transform( const GeoMatrix& matrix ){
    GeoMatrix m = matrix;
    for( int i = 0 ; i < vertices.size(); i++ ){
        vertices[i].position = (m * GeoVector( vertices[i].position )).ToGeoFloat3();
    }
    return *this;
}

Shape& Shape::TranslateAlongNormal( const float distance ){
    for( int i = 0 ; i < vertices.size(); i++ ){
        vertices[i].position = (GeoMatrix::Translation(GeoVector(vertices[i].normal) * distance) * GeoVector( vertices[i].position )).ToGeoFloat3();
    }
    return *this;
}




