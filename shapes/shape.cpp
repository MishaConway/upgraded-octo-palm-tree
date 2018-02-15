#include "shape.h"
#include "shape_utilities/tangents.h"
#include "shape_utilities/tgen.h"






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

    return average;
}


Vertex Vertex::Average( const std::vector<Vertex>& vertices ){
    GeoVector average_position;
    GeoVector average_color_uv;
    GeoVector average_normal;
    for( int i = 0; i < vertices.size(); i++ ){
        average_position += GeoVector( vertices[i].position );
        average_color_uv += GeoVector( vertices[i].colorUV );
        average_normal += GeoVector( vertices[i].normal );
    }
    
    average_position /= vertices.size();
    average_color_uv /= vertices.size();
    average_normal /= vertices.size();
    
    return Vertex( average_position.ToGeoFloat3(), average_color_uv.ToGeoFloat2(), average_normal.ToGeoFloat3());
}




Shape::~Shape(){
    
}

std::vector<Vertex> Shape::ToVertices(){
    auto verts = ToVerticesEx();
    CalculateTangentArray( verts );
    
    /*
    std::vector<tgen::VIndexT> position_indices;
    std::vector<tgen::VIndexT> uv_indices;
    std::vector<tgen::RealT> positions;
    std::vector<tgen::RealT> uvs;
    std::vector<tgen::RealT> normals;
    std::vector<tgen::RealT> tangents;
    std::vector<tgen::RealT> bitangents;



    for( int i = 0; i < verts.size(); i++ ){
        auto v = verts[i];
        
        position_indices.push_back(i);
        uv_indices.push_back(i);
        
        positions.push_back(v.position.x);
        positions.push_back(v.position.y);
        positions.push_back(v.position.z);
        
        normals.push_back(v.normal.x);
        normals.push_back(v.normal.y);
        normals.push_back(v.normal.z);

        uvs.push_back(v.colorUV.x);
        uvs.push_back(v.colorUV.y);
    }

    tgen::computeCornerTSpace(position_indices,
                             uv_indices,
                             positions,
                             uvs,
                             tangents,
                              bitangents);
    
    
    for( int i = 0; i < verts.size(); i++ ){
        verts[i].tangent = GeoFloat4( tangents[i*3], tangents[i*3+1], tangents[i*3+2], 1 );
        printf( "normal is %f, %f, %f\n", verts[i].normal.x, verts[i].normal.y,verts[i].normal.z);
        printf( "tangent is %f, %f, %f\n", verts[i].tangent.x, verts[i].tangent.y, verts[i].tangent.z );
    }

    
    std::vector<tgen::RealT> tangents4d;
    
    tgen::computeTangent4D( normals,
                     tangents,
                     bitangents,
                           tangents4d);
    
    
    //for( int i = 0; i < verts.size(); i++ ){
     //   verts[i].tangent = GeoFloat4( tangents4d[i*4], tangents4d[i*4+1], tangents4d[i*4+2], tangents4d[i*4+3] );
   // }

    
    */
    
    
    
    
    return verts;
}

Shape& Shape::Transform( const GeoMatrix& matrix ){
    GeoMatrix m = matrix;
    for( int i = 0 ; i < vertices.size(); i++ ){
        vertices[i].position = (m * GeoVector( vertices[i].position )).ToGeoFloat3();
    }
    return *this;
}

Shape& Shape::TranslateAlongNormal( const float distance ){
    for( int i = 0 ; i < vertices.size(); i++ ){
        vertices[i].position = (GeoMatrix::Translation(GeoVector(vertices[i].normal) * distance) * GeoVector( vertices[i].position )).ToGeoFloat3();
    }
    return *this;
}




