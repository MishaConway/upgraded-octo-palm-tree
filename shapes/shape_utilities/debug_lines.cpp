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

std::pair< GeoFloat2, GeoFloat2 > FindMinMaxUvs( std::vector<Vertex> vertices ){
    bool min_x_set = false;
    bool max_x_set = false;
    bool min_y_set = false;
    bool max_y_set = false;
    GeoFloat2 min, max;
    
    
    for( int i = 0; i < vertices.size(); i++ ){
        auto uv = vertices[i].colorUV;
        
        if( !min_x_set || uv.x < min.x ){
            min_x_set = true;
            min.x = uv.x;
        }
        
        if( !max_x_set || uv.x > max.x ){
            max_x_set = true;
            max.x = uv.x;
        }
        
        if( !min_y_set || uv.y < min.y ){
            min_y_set = true;
            min.y = uv.y;
        }
        
        if( !max_y_set || uv.y > max.y ){
            max_y_set = true;
            max.y = uv.y;
        }
    }
    
    
    return std::pair< GeoFloat2, GeoFloat2 >( min, max );
}



