#pragma once

#include "shape.h"



class Cube : public Shape
{
public:
    virtual Cube();
    Cube();
    Cube( const GeoFloat3& origin, width, height, length );
    
    static Cube UnitCube();
    
    virtual std::vector<Vertex> ToVertices();
    virtual std::vector<vert3> Triangulate();
    virtual void ReverseWinding();
private:
        
};