#pragma once

#include "../geo/GeoVector.h"
#include <vector>
#include <string>
#include <math.h>
#include "../std/triple.h"


struct Vertex
{
    GeoFloat3 position;
    GeoFloat3 colorUV;
    GeoFloat3 normal;
    GeoFloat4 tangent;
    GeoFloat3 bitangent;
    Vertex();
    Vertex( GeoFloat3 position );
    Vertex( GeoFloat3 position, GeoFloat2 uv );
    Vertex( GeoFloat3 position, GeoFloat2 uv, GeoFloat3 normal );
    Vertex( GeoVector vector );
    static Vertex Average( const Vertex& a, const Vertex& b );
    static Vertex Average( const std::vector<Vertex>& vertices );
};

typedef std::pair<Vertex, Vertex> vert2;
typedef std::triple<Vertex, Vertex, Vertex> vert3;


std::vector<Vertex> UnindexVertices( const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices );