#include "shape.h"

Vertex::Vertex(){
    colorUV.z = 0;
}

Vertex::Vertex( GeoFloat3 position ){
    this->position = position;
}

Vertex::Vertex( GeoFloat3 position, GeoFloat2 uv ){
    this->position = position;
    this->colorUV.x = uv.x;
    this->colorUV.y = uv.y;
    this->colorUV.z = 0;
}

Vertex::Vertex( GeoVector vector ){
    position = vector.ToGeoFloat3();
}

Shape::~Shape(){
    
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




