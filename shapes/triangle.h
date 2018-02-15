#pragma once

#include "shape.h"



class Triangle : public Shape
{
public:
    virtual ~Triangle();
    Triangle( const Vertex& a, const Vertex& b, const Vertex& c );
    Triangle( const GeoFloat3& a, const GeoFloat3& b, const GeoFloat3& c );
    Triangle( const GeoVector& a, const GeoVector& b, const GeoVector& c );
    Triangle( const vert3 vertices );
    bool PointInsideXYTriangle( const GeoFloat2& point );
    GeoVector ComputeNormal();
    virtual std::vector<Vertex> ToVerticesEx();
    virtual std::vector<vert3> Triangulate();
    virtual Shape& ReverseWinding();
    
    float SignedArea( const GeoVector& reference_normal );
    bool IsClockwise( const GeoVector& reference_normal );
    bool IsCounterClockwise( const GeoVector& reference_normal );
private:
    
};