#include "quad.h"
#include "triangle.h"

Quad::~Quad(){
    
}

Quad::Quad(){
    vertices.resize(4);
}


Quad::Quad( const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d ){
    vertices.resize(4);
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    vertices[3] = d;
}

Quad Quad::XZQuad( GeoFloat3 a, const float width, const float length )
{
    Quad quad;
    for( unsigned int i = 0; i < 4; i++ )
    {
        quad.vertices[i].position = a;
        quad.vertices[i].colorUV.x = 0;
        quad.vertices[i].colorUV.y = 1;
    }
    
    quad.vertices[1].position.x += width;
    quad.vertices[1].colorUV.x = 1;
    
    quad.vertices[2].position.x += width;
    quad.vertices[2].position.z += length;
    quad.vertices[2].colorUV.x = 1;
    quad.vertices[2].colorUV.y = 0;
    
    quad.vertices[3].position.z += length;
    quad.vertices[3].colorUV.y = 0;
    
    quad.ComputeNormals();
    
    return quad;
}

Quad Quad::XZQuadCentered( GeoFloat3 a, const float width, const float length ){
    Quad quad = Quad::XZQuad(a, width, length );
    for( unsigned int i = 0; i < 4; i++ ){
        quad.vertices[i].position.x -= width / 2.0f;
        quad.vertices[i].position.z -= length / 2.0f;
    }
    return quad;
}

Quad Quad::XZUnitQuad(){
    return Quad::XZQuadCentered(GeoFloat3(0, 0, 0), 1, 1);
}

Quad Quad::XYQuad( GeoFloat3 a, const float width, const float height )
{
    Quad quad;
    for( unsigned int i = 0; i < 4; i++ )
    {
        quad.vertices[i].position = a;
        quad.vertices[i].colorUV.x = quad.vertices[i].colorUV.y = 0;
    }
    
    quad.vertices[1].position.x += width;
    quad.vertices[1].colorUV.x = 1;
    
    quad.vertices[2].position.x += width;
    quad.vertices[2].position.y += height;
    quad.vertices[2].colorUV.x = quad.vertices[2].colorUV.y = 1;
    
    quad.vertices[3].position.y += height;
    quad.vertices[3].colorUV.y = 1;
    
    quad.ComputeNormals();

    return quad;
}

Quad Quad::XYQuadCentered( GeoFloat3 a, const float width, const float height ){
    Quad quad = Quad::XYQuad(a, width, height );
    for( unsigned int i = 0; i < 4; i++ ){
        quad.vertices[i].position.x -= width / 2.0f;
        quad.vertices[i].position.y -= height / 2.0f;
    }
    return quad;
}

Quad Quad::XYUnitQuad(){
    return Quad::XYQuadCentered(GeoFloat3(0, 0, 0), 1, 1);
}

Quad Quad::ZYQuad( GeoFloat3 a, const float length, const float height )
{
    Quad quad;
    for( unsigned int i = 0; i < 4; i++ )
    {
        quad.vertices[i].position = a;
        quad.vertices[i].colorUV.x = quad.vertices[i].colorUV.y = 0;
    }
    
    quad.vertices[1].position.z += length;
    quad.vertices[1].colorUV.x = 1;
    
    quad.vertices[2].position.y += height;
    quad.vertices[2].position.z += length;
    quad.vertices[2].colorUV.x = quad.vertices[2].colorUV.y = 1;
    
    quad.vertices[3].position.y += height;
    quad.vertices[3].colorUV.y = 1;
    quad.ReverseWinding();
    
    quad.ComputeNormals();
    
    
    return quad;
}

Quad Quad::ZYQuadCentered( GeoFloat3 a, const float length, const float height ){
    Quad quad = Quad::ZYQuad(a, length, height );
    for( unsigned int i = 0; i < 4; i++ ){
        quad.vertices[i].position.y -= height / 2.0f;
        quad.vertices[i].position.z -= length / 2.0f;
    }
    return quad;
}

Quad Quad::ZYUnitQuad(){
    return Quad::ZYQuadCentered(GeoFloat3(0, 0, 0), 1, 1);
}

std::vector<Vertex> Quad::ToVertices(){
    auto tris = Triangulate();
    std::vector<Vertex> verts;
    for( int i = 0; i < tris.size(); i++ ){
        verts.push_back(tris[i].first);
        verts.push_back(tris[i].second);
        verts.push_back(tris[i].third);
    }
    return verts;
}

std::vector<vert3> Quad::Triangulate()
{
    std::vector<vert3> tris;
    tris.push_back( vert3( vertices[2], vertices[1], vertices[0]) );
    tris.push_back( vert3( vertices[3], vertices[2], vertices[0]) );
    return tris;
}

Shape& Quad::ReverseWinding()
{
    Vertex temp = vertices[3];
    vertices[3] = vertices[1];
    vertices[1] = temp;
    for( int i = 0; i < vertices.size(); i++ )
        vertices[i].normal = (GeoVector( vertices[i].normal ) * -1).ToGeoFloat3();
    return *this;
}

void Quad::ComputeNormals(){
    GeoFloat3 normal = Triangle( Triangulate().front() ).ComputeNormal().ToGeoFloat3();
    for( unsigned int i = 0; i < 4; i++ )
        vertices[i].normal = normal;
}