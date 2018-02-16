#pragma once

#include "vertices.h"
#include "../geo/GeoMatrix.h"



class Shape {
public:
    virtual ~Shape();
    virtual std::vector<Vertex> ToVertices();
    virtual std::vector<Vertex> ToVerticesEx() = 0;
    virtual std::vector<vert3> Triangulate() = 0;
    virtual Shape& ReverseWinding() = 0;
    virtual Shape& Transform( const GeoMatrix& matrix );
    virtual Shape& TranslateAlongNormal( const float distance );
    

protected:
    std::vector<Vertex> vertices;
};