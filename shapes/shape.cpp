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

Vertex::Vertex( GeoFloat3 position, GeoFloat2 uv, GeoFloat3 normal ){
    this->position = position;
    this->colorUV.x = uv.x;
    this->colorUV.y = uv.y;
    this->colorUV.z = 0;
    this->normal = normal;
}

Vertex::Vertex( GeoVector vector ){
    position = vector.ToGeoFloat3();
}

Vertex Vertex::Average( const Vertex& a, const Vertex& b ){
    Vertex average;
    average.position = ((GeoVector( a.position ) + GeoVector(b.position)) / 2.0f).ToGeoFloat3();
    average.colorUV = ((GeoVector( a.colorUV ) + GeoVector(b.colorUV)) / 2.0f).ToGeoFloat3();
    average.normal = ((GeoVector( a.normal ) + GeoVector(b.normal)) / 2.0f).ToGeoFloat3();

    return average;
}


Vertex Vertex::Average( const std::vector<Vertex>& vertices ){
    GeoVector average_position;
    GeoVector average_color_uv;
    GeoVector average_normal;
    for( int i = 0; i < vertices.size(); i++ ){
        average_position += GeoVector( vertices[i].position );
        average_color_uv += GeoVector( vertices[i].colorUV );
        average_normal += GeoVector( vertices[i].normal );
    }
    
    average_position /= vertices.size();
    average_color_uv /= vertices.size();
    average_normal /= vertices.size();
    
    return Vertex( average_position.ToGeoFloat3(), average_color_uv.ToGeoFloat2(), average_normal.ToGeoFloat3());
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




