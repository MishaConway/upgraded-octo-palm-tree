#include "vertices.h"

Vertex::Vertex(){
    colorUV.z = 0;
}

Vertex::Vertex( GeoFloat3 position ){
    this->position = position;
}

Vertex::Vertex( GeoFloat3 position, GeoFloat2 uv ){
    this->position = position;
    this->colorUV.x = uv.x;
    this->colorUV.y = uv.y;
    this->colorUV.z = 0;
}

Vertex::Vertex( GeoFloat3 position, GeoFloat2 uv, GeoFloat3 normal ){
    this->position = position;
    this->colorUV.x = uv.x;
    this->colorUV.y = uv.y;
    this->colorUV.z = 0;
    this->normal = normal;
}

Vertex::Vertex( GeoVector vector ){
    position = vector.ToGeoFloat3();
}

Vertex Vertex::Average( const Vertex& a, const Vertex& b ){
    Vertex average;
    average.position = ((GeoVector( a.position ) + GeoVector(b.position)) / 2.0f).ToGeoFloat3();
    average.colorUV = ((GeoVector( a.colorUV ) + GeoVector(b.colorUV)) / 2.0f).ToGeoFloat3();
    average.normal = ((GeoVector( a.normal ) + GeoVector(b.normal)) / 2.0f).ToGeoFloat3();
    average.tangent = ((GeoVector( a.tangent ) + GeoVector(b.tangent)) / 2.0f).ToGeoFloat4();
    average.bitangent = ((GeoVector( a.bitangent ) + GeoVector(b.bitangent)) / 2.0f).ToGeoFloat3();
    return average;
}


Vertex Vertex::Average( const std::vector<Vertex>& vertices ){
    GeoVector average_position;
    GeoVector average_color_uv;
    GeoVector average_normal, average_tangent, average_bitangent;
    for( int i = 0; i < vertices.size(); i++ ){
        average_position += GeoVector( vertices[i].position );
        average_color_uv += GeoVector( vertices[i].colorUV );
        average_normal += GeoVector( vertices[i].normal );
        average_tangent += GeoVector( vertices[i].tangent );
        average_bitangent += GeoVector( vertices[i].bitangent );

    }
    
    average_position /= vertices.size();
    average_color_uv /= vertices.size();
    average_normal /= vertices.size();
    average_tangent /= vertices.size();
    average_bitangent /= vertices.size();
    
    auto vert = Vertex( average_position.ToGeoFloat3(), average_color_uv.ToGeoFloat2(), average_normal.ToGeoFloat3());
    vert.tangent = average_tangent.ToGeoFloat4();
    vert.bitangent = average_bitangent.ToGeoFloat3();
    
    return vert;
}

std::vector<Vertex> UnindexVertices( const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices ){
    std::vector<Vertex> unindexed_vertices;
    
    for( int i = 0; i < indices.size(); i += 3 ){
        auto index_a = indices[i];
        auto index_b = indices[i+1];
        auto index_c = indices[i+2];
        
        unindexed_vertices.push_back( vertices[index_a] );
        unindexed_vertices.push_back( vertices[index_b] );
        unindexed_vertices.push_back( vertices[index_c] );
    }
    
    return unindexed_vertices;
}

std::triple< std::vector<GeoFloat3>, std::vector<GeoVector>, std::vector<GeoFloat2> > DecomposeVertices( const std::vector<Vertex>& vertices ){
    
    std::vector<GeoFloat3> positions;
    std::vector<GeoVector> normals;
    std::vector<GeoFloat2> texcoords;
    
    for( int i = 0; i < vertices.size(); i++ ){
        positions.push_back(vertices[i].position);
        normals.push_back(GeoVector(vertices[i].normal));
        texcoords.push_back(GeoFloat2(vertices[i].colorUV));
    }
    
    return std::triple<std::vector<GeoFloat3>, std::vector<GeoVector>, std::vector<GeoFloat2>>( positions, normals, texcoords );
}

std::pair< std::vector<Vertex>, std::vector<unsigned int> > DuplicateVerticesOnNormalCreaseAngle(
    const float normal_crease_angle,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices
){
    auto duplicated_vertices = vertices;
    auto duplicated_indices = indices;
    
    std::vector<GeoVector> face_averaged_normals;
    face_averaged_normals.resize( vertices.size() );
    
    for (long a = 0; a < vertices.size(); a += 3)
    {
        int nCount[5] = {0, 1, 2, 3, 4};
        
        auto p1 = GeoVector(vertices[a].position);
        auto p2 = GeoVector(vertices[a+1].position);
        auto p3 = GeoVector(vertices[a+2].position);
        
        auto e1 = p1 - p2;
        auto e2 = p3 - p2;
        auto face_normal = e1.Cross(e2);
        
        face_averaged_normals[a] += face_normal;
        face_averaged_normals[a+1] += face_normal;
        face_averaged_normals[a+2] += face_normal;
    }
    
    
    
    
    
    
    
    
    return std::pair< std::vector<Vertex>, std::vector<unsigned int> >( duplicated_vertices, duplicated_indices );
}



