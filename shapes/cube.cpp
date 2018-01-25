#include "cube.h"

Cube::~Cube(){
    
}

Cube::Cube(){
    
}

Cube::Cube( const GeoFloat3& origin, const float width, const float height, const float length ){
    //top
    quads.push_back( dynamic_cast<Quad&>(Quad::XZUnitQuad().TranslateAlongNormal( 0.5f )) );
    
    //bottom
    quads.push_back( dynamic_cast<Quad&>(Quad::XZUnitQuad().ReverseWinding().TranslateAlongNormal( 0.5f )) );
    
    //front
    quads.push_back( dynamic_cast<Quad&>(Quad::XYUnitQuad().TranslateAlongNormal( 0.5f )) );

    //back
    quads.push_back( dynamic_cast<Quad&>(Quad::XYUnitQuad().ReverseWinding().TranslateAlongNormal( 0.5f )) );
    
    //right
    quads.push_back( dynamic_cast<Quad&>(Quad::ZYUnitQuad().TranslateAlongNormal( 0.5f )) );

    //left
    quads.push_back( dynamic_cast<Quad&>(Quad::ZYUnitQuad().ReverseWinding().TranslateAlongNormal( 0.5f )) );
    
    
    Transform( GeoMatrix::Translation( origin ) );
    //Transform( GeoMatrix::Scaling(width, height, length) );
}

Cube Cube::UnitCube(){
    return Cube( GeoFloat3(0,0,0), 1, 1, 1);
}

std::vector<Vertex> Cube::ToVertices(){
    std::vector<Vertex> verts;
    for( int i = 0; i < quads.size(); i++ ){
        auto quad_vertices = quads[i].ToVertices();
        for( int j = 0; j < quad_vertices.size(); j++ )
            verts.push_back(quad_vertices[j]);
    }
    return verts;
}



std::vector<vert3> Cube::Triangulate(){
    std::vector<vert3> tris;
    for( int i = 0; i < quads.size(); i++ ){
        auto quad_tris = quads[i].Triangulate();
        for( int j = 0; j < quad_tris.size(); j++ )
            tris.push_back(quad_tris[j]);
    }
    return tris;
    
}

Shape& Cube::ReverseWinding(){
    for( int i = 0; i < quads.size(); i++ )
        quads[i].ReverseWinding();
    return *this;
}

Shape& Cube::Transform( const GeoMatrix& matrix ){
    for( int i = 0; i < quads.size(); i++ )
        quads[i].Transform(matrix);
    return *this;
}

Shape& Cube::TranslateAlongNormal( const float distance ){
    for( int i = 0; i < quads.size(); i++ )
        quads[i].TranslateAlongNormal(distance);
    return *this;
}