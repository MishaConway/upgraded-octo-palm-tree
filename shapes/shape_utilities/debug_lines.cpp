#include "debug_lines.h"

std::vector<Vertex>  CalculateDebugNormalLines( std::vector<Vertex> vertices, const float length ){
    std::vector< Vertex > line_vertices;
    for( int i = 0; i < vertices.size(); i++ ){
        Vertex a = vertices[i];
        Vertex b = vertices[i];
        b.position = (GeoVector( b.position ) + GeoVector( b.normal ) * length).ToGeoFloat3();
        line_vertices.push_back(a);
        line_vertices.push_back(b);
    }
    return line_vertices;
}

std::vector<Vertex> CalculateDebugTangentLines( std::vector<Vertex> vertices, const float length ){
    std::vector< Vertex > line_vertices;
    for( int i = 0; i < vertices.size(); i++ ){
        Vertex a = vertices[i];
        Vertex b = vertices[i];
        b.position = (GeoVector( b.position ) + GeoVector( b.tangent ) * length).ToGeoFloat3();
        line_vertices.push_back(a);
        line_vertices.push_back(b);
    }
    return line_vertices;
    
}

std::vector<Vertex> CalculateDebugBiTangentLines( std::vector<Vertex> vertices, const float length ){
    std::vector< Vertex > line_vertices;
    for( int i = 0; i < vertices.size(); i++ ){
        Vertex a = vertices[i];
        Vertex b = vertices[i];
        b.position = (GeoVector( b.position ) + GeoVector( b.bitangent ) * length).ToGeoFloat3();
        line_vertices.push_back(a);
        line_vertices.push_back(b);
    }
    return line_vertices;
    
}
