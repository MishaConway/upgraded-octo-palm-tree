#include "triangle.h"

Triangle::~Triangle(){
    
}

Triangle::Triangle( const Vertex& a, const Vertex& b, const Vertex& c ){
    vertices.resize(3);
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
}


Triangle::Triangle( const GeoFloat3& a, const GeoFloat3& b, const GeoFloat3& c ){
    vertices.resize(3);
    this->vertices[0] = Vertex(a);
    this->vertices[1] = Vertex(b);
    this->vertices[2] = Vertex(c);
}

Triangle::Triangle( const GeoVector& a, const GeoVector& b, const GeoVector& c ){
    vertices.resize(3);
    this->vertices[0] = Vertex(a);
    this->vertices[1] = Vertex(b);
    this->vertices[2] = Vertex(c);
}


Triangle::Triangle( const vert3 vertices ){
    this->vertices.resize(3);
    this->vertices[0] = vertices.first;
    this->vertices[1] = vertices.second;
    this->vertices[2] = vertices.third;
}

bool Triangle::PointInsideXYTriangle( const GeoFloat2& point ){
    GeoVector A = GeoVector(vertices[0].position);
    GeoVector B = GeoVector(vertices[1].position);
    GeoVector C = GeoVector(vertices[2].position);
    GeoVector P( point.x, point.y );
    
    
    
    // Compute vectors
    GeoVector v0 = C - A;
    GeoVector v1 = B - A;
    GeoVector v2 = P - A;
    
    // Compute dot products
    float dot00 = v0.Dot(v0);
    float dot01 = v0.Dot(v1);
    float dot02 = v0.Dot(v2);
    float dot11 = v1.Dot(v1);
    float dot12 = v1.Dot(v2);
    
    // Compute barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

GeoVector Triangle::ComputeNormal(){
    return ( GeoVector( vertices[0].position ) - GeoVector( vertices[1].position) ).Cross( GeoVector( vertices[2].position ) - GeoVector( vertices[0].position) ).Normalize();
}


std::vector<Vertex> Triangle::ToVertices(){
    return vertices;
}

std::vector<vert3> Triangle::Triangulate(){
    std::vector<vert3> tris;
    tris.push_back( vert3( vertices[0], vertices[1], vertices[2]) );
    return tris;
}

void Triangle::ReverseWinding(){
    Vertex temp = vertices[0];
    vertices[0] = vertices[2];
    vertices[2] = temp;
}




