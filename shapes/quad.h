#pragma once

#include "shape.h"



class Quad : public Shape
{
public:
    virtual ~Quad();
    
    
    static Quad XZQuad( GeoFloat3 a, const float width, const float length );
    static Quad XZQuadCentered( GeoFloat3 a, const float width, const float length );
    static Quad XZUnitQuad();
    
    static Quad XYQuad( GeoFloat3 a, const float width, const float height );
    static Quad XYQuadCentered( GeoFloat3 a, const float width, const float height );
    static Quad XYUnitQuad();
    
    
    static Quad ZYQuad( GeoFloat3 a, const float length, const float height );
    static Quad ZYQuadCentered( GeoFloat3 a, const float length, const float height );
    static Quad ZYUnitQuad();
    
    
    void SquareTesselate();
    
    
    Quad();
    Quad( const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d );
   
    virtual std::vector<Vertex> ToVerticesEx();
    virtual std::vector<vert3> Triangulate();
    virtual Shape& ReverseWinding();
private:
    void ComputeNormals();
private:
    std::vector<Quad> quads;
   
};