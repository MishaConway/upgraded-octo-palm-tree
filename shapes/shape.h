#pragma once

#include "../geo/GeoFloat.h"
#include "../geo/GeoVector.h"
#include "../geo/GeoMatrix.h"
#include "../geo/GeoQuaternion.h"
#include <vector>
#include <string>
#include <math.h>
#include "glew.h"
#include "../std/triple.h"


struct Vertex
{
    GeoFloat3 position;
    GeoFloat3 colorUV;
    GeoFloat3 normal;
    GeoFloat3 tangent;
    GeoFloat3 bitangent;
    Vertex();
    Vertex( GeoFloat3 position );
    Vertex( GeoFloat3 position, GeoFloat2 uv );
    Vertex( GeoVector vector );
};

typedef std::pair<Vertex, Vertex> vert2;
typedef std::triple<Vertex, Vertex, Vertex> vert3;


class Shape {
public:
    virtual ~Shape();
    virtual std::vector<Vertex> ToVertices() = 0;
    virtual std::vector<vert3> Triangulate() = 0;
    virtual Shape& ReverseWinding() = 0;
    virtual Shape& Transform( const GeoMatrix& matrix );
    virtual Shape& TranslateAlongNormal( const float distance );
    

protected:
    std::vector<Vertex> vertices;
};