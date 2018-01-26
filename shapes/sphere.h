#pragma once

#include "shape.h"
#include "triangle.h"

class Sphere : public Shape
{
public:
    virtual ~Sphere();
    Sphere();
   
    virtual std::vector<Vertex> ToVertices();
    virtual std::vector<vert3> Triangulate();
    virtual Shape& ReverseWinding();
private:
    std::vector<Triangle> triangles;
};