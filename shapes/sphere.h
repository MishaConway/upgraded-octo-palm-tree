#pragma once

#include "shape.h"
#include "triangle.h"

class Sphere : public Shape
{
public:
    virtual ~Sphere();
    Sphere();
   
    virtual std::vector<Vertex> ToVertices();
    virtual std::vector<Vertex> ToVerticesEx();
    virtual std::vector<vert3> Triangulate();
    virtual Shape& ReverseWinding();
    virtual Shape& Transform( const GeoMatrix& matrix );
    virtual Shape& TranslateAlongNormal( const float distance );
private:
    std::vector<Triangle> triangles;
};