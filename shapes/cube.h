#pragma once

#include "shape.h"
#include "quad.h"



class Cube : public Shape
{
public:
    virtual ~Cube();
    Cube( const GeoFloat3& origin, const float width, const float height, const float length );
    static Cube UnitCube();
    
    virtual std::vector<Vertex> ToVertices();
    virtual std::vector<vert3> Triangulate();
    virtual Shape& ReverseWinding();
    
    virtual Shape& Transform( const GeoMatrix& matrix );
    virtual Shape& TranslateAlongNormal( const float distance );
protected:
    Cube();
    std::vector<Quad> quads;
};