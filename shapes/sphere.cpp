#include "sphere.h"
#include "shape_utilities/ico_sphere.h"
#include "shape_utilities/cry_tangents.h"
#include "shape_utilities/tangents.h"

struct CryTriangleInput
{
    unsigned int GetTriangleCount() const{
        return (unsigned int) indices.size() / 3;
    }
    
    
    void GetTriangleIndices( const unsigned int indwTriNo, unsigned int outdwPos[3], unsigned int outdwNorm[3], unsigned int outdwUV[3] ) const{
        
        outdwPos[2] = indices[indwTriNo*3];
        outdwPos[1] = indices[indwTriNo*3 + 1];
        outdwPos[0] = indices[indwTriNo*3 + 2];
        
        outdwNorm[2] = indices[indwTriNo*3];
        outdwNorm[1] = indices[indwTriNo*3 + 1];
        outdwNorm[0] = indices[indwTriNo*3 + 2];
        
        outdwUV[2] = indices[indwTriNo*3];
        outdwUV[1] = indices[indwTriNo*3 + 1];
        outdwUV[0] = indices[indwTriNo*3 + 2];
    }
    
    void GetPos( const unsigned int indwPos, float outfPos[3] ) const{
        auto vertex = vertices[indwPos];
        outfPos[0] = vertex.position.x;
        outfPos[1] = vertex.position.y;
        outfPos[2] = vertex.position.z;
    }
    
    
    void GetUV( const unsigned int indwPos, float outfUV[2] ) const{
        auto vertex = vertices[indwPos];
        outfUV[0] = vertex.colorUV.x;
        outfUV[1] = vertex.colorUV.y;
    }
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

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
        
        auto tan = tangents[i];
        auto bitan = bitangents[i];
        
        
        if( tan.w < 0 ){
        
        }
        else {
            //bitan *= -1;
            //tan *= -1;
        }
        
        
        v.tangent = tan.ToGeoFloat4();
        v.bitangent = bitan.ToGeoFloat3();
        verts.push_back(v);
    }
    
    for( int i = 0; i < icoSphere.indices.size(); i += 3 ){
        auto index_a = icoSphere.indices[i];
        auto index_b = icoSphere.indices[i+1];
        auto index_c = icoSphere.indices[i+2];
        
        auto a = verts[index_a];
        auto b = verts[index_b];
        auto c = verts[index_c];
        
        triangles.push_back( Triangle( a, b, c) );
    }
    
    
    return;
    
    CryTriangleInput cry;
    for( int i = 0; i < icoSphere.vertices.size(); i++ ){
        auto vert = Vertex( icoSphere.vertices[i] );
        vert.normal = GeoVector(vert.position).Normalize().ToGeoFloat3();
        vert.colorUV.x = asinf(vert.position.x) / M_PI+0.5f;
        vert.colorUV.y = -(asinf(vert.position.y) / M_PI+0.5f);
        cry.vertices.push_back( vert);
    }
    cry.indices = icoSphere.indices;
    
    printf( "num indices is %i\n", cry.indices.size() );
    
    
    CTangentSpaceCalculation<CryTriangleInput> MyTangent;
    MyTangent.CalculateTangentSpace(cry);
    
    int times_inverted = 0;
    int times_not_inverted = 0;
    for( int tri = 0; tri < cry.GetTriangleCount(); tri++ ){
        unsigned int base_indices[3];

        MyTangent.GetTriangleBaseIndices( tri, base_indices );

        for( int base_i = 0; base_i < 3; base_i++ ){
            auto base_index = base_indices[base_i];
            auto vert = cry.vertices[cry.indices[tri*3 + base_i]];
            MyTangent.GetBase(base_index, (float*) &vert.tangent, (float*) &vert.bitangent, (float*) &vert.normal);
            //if (glm::dot(glm::cross(n, t), b) < 0.0f){
            //    t = t * -1.0f;
            //}
            
            
         
        
            if( GeoVector(vert.normal).Cross( GeoVector(vert.tangent) ).Dot( GeoVector(vert.bitangent) ) < 0 ){
                vert.tangent = (GeoVector(vert.tangent) * -1).ToGeoFloat4();
                times_inverted++;
            } else {
                vert.bitangent = (GeoVector(vert.bitangent) * -1).ToGeoFloat3();

                times_not_inverted++;
            }
            
            
          
            
            
            
            verts.push_back(vert);
        }
    }
    
    printf( "times inverted is %i\n", times_inverted);
    printf( "times not inverted is %i\n", times_not_inverted);

    
    for( int i = 0; i < verts.size(); i += 3 ){
        auto a = verts[i];
        auto b = verts[i+1];
        auto c = verts[i+2];
        
        triangles.push_back( Triangle( a, b, c) );
    }
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






