#include "sphere.h"
#include "shape_utilities/ico_sphere.h"

Sphere::~Sphere(){
    
}

Sphere::Sphere(){
    ShapeUtilities::IcoSphere icoSphere;
    icoSphere.Create(2);
    
    std::vector<Vertex> verts;
    for( int i = 0; i < icoSphere.indices.size(); i++ ){
        auto index = icoSphere.indices[i];
        Vertex vertex =  Vertex( icoSphere.vertices[index] );
        vertex.normal = GeoVector(vertex.position).Normalize().ToGeoFloat3();
        
        //spherical mapping with normals
        vertex.colorUV.x = asinf(vertex.normal.x) / M_PI+0.5f;
        vertex.colorUV.y = asinf(vertex.normal.y) / M_PI+0.5f;
        
        //spherical mapping with positions
        vertex.colorUV.x = asinf(vertex.position.x) / M_PI+0.5f;
        vertex.colorUV.y = asinf(vertex.position.y) / M_PI+0.5f;
        
        
        verts.push_back( vertex );
    }
    
    for( int i = 0; i < verts.size(); i += 3 ){
        auto a = verts[i];
        auto b = verts[i+1];
        auto c = verts[i+2];
        
        triangles.push_back( Triangle( a, b, c) );
    }
}

std::vector<Vertex> Sphere::ToVertices(){
    std::vector<Vertex> verts;
    for( int i = 0; i < triangles.size(); i++ ){
        auto triangle_vertices = triangles[i].ToVertices();
        for( int j = 0; j < triangle_vertices.size(); j++ )
            verts.push_back(triangle_vertices[j]);
    }
    return verts;
}

std::vector<vert3> Sphere::Triangulate(){
    std::vector<vert3> tris;
    return tris;

}

Shape& Sphere::ReverseWinding(){
    return *this;
}






