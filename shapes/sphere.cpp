#include "sphere.h"
#include "shape_utilities/ico_sphere.h"
#include "shape_utilities/cry_tangents.h"
#include "shape_utilities/tangents.h"


Sphere::~Sphere(){
    
}

Sphere::Sphere(){
    ShapeUtilities::IcoSphere icoSphere;
    icoSphere.Create(3);
    
    std::vector<GeoFloat3> positions;
    std::vector<GeoFloat2> texcoords;
    std::vector<GeoVector> normals;
    
    for( int i = 0; i < icoSphere.vertices.size(); i++ ){
        auto position = icoSphere.vertices[i].ToGeoFloat3();
        positions.push_back( position );
        normals.push_back( icoSphere.vertices[i].Normalize() );
        texcoords.push_back( GeoFloat2( asinf(position.x) / M_PI+0.5f, asinf(position.y) / M_PI+0.5f) );
    }
    
    auto tan_bitans = CalculateTangentArray( positions, normals, texcoords, icoSphere.indices );
    auto tangents = tan_bitans.first;
    auto bitangents = tan_bitans.second;
    
    std::vector<Vertex> verts;
    for( int i = 0; i < icoSphere.indices.size(); i++ ){
        Vertex v;
        v.position = positions[i];
        v.colorUV = GeoFloat3(texcoords[i]);
        v.normal = normals[i].ToGeoFloat3();
        v.tangent = tangents[i].ToGeoFloat4();
        v.bitangent = bitangents[i].ToGeoFloat3();
        verts.push_back(v);
    }
    
    triangles = Triangle::ListFromIndexedVertices(verts, icoSphere.indices);
}

std::vector<Vertex> Sphere::ToVerticesEx(){
    std::vector<Vertex> verts;
    for( int i = 0; i < triangles.size(); i++ ){
        auto triangle_vertices = triangles[i].ToVerticesEx();
        for( int j = 0; j < triangle_vertices.size(); j++ )
            verts.push_back(triangle_vertices[j]);
    }
    return verts;
}

std::vector<Vertex> Sphere::ToVertices(){
    return ToVerticesEx();
}

std::vector<vert3> Sphere::Triangulate(){
    std::vector<vert3> tris;
    return tris;

}

Shape& Sphere::ReverseWinding(){
    for( int i = 0; i < triangles.size(); i++ ){
        triangles[i].ReverseWinding();
    }
    
    return *this;
}

Shape& Sphere::Transform( const GeoMatrix& matrix ){
    for( int i = 0; i < triangles.size(); i++ ){
        triangles[i].Transform(matrix);
    }
    
    return *this;
}

Shape& Sphere::TranslateAlongNormal( const float distance ){
    for( int i = 0; i < triangles.size(); i++ ){
        triangles[i].TranslateAlongNormal( distance );
    }
  
    return *this;
}






