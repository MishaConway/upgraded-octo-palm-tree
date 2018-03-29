#include "tangents.h"


//http://www.terathon.com/code/tangent.html


struct Triangle
{
    unsigned short  index[3];
};



std::pair< std::vector<GeoVector>, std::vector<GeoVector> > CalculateMirroredSphereTangentArray(
                                                                                    std::vector<GeoFloat3> vertex,
                                                                                    std::vector<GeoVector> normal,
                                                                                    std::vector<GeoFloat2> texcoord,
                                                                                    std::vector<unsigned int> indices
                                                                                                )
{
    auto vertexCount = vertex.size();
    auto triangleCount = indices.size() / 3;
    
    std::vector<GeoVector> tangents, bitangents;
    tangents.resize( vertexCount );
    bitangents.resize( vertexCount );
    
    for (long a = 0; a < triangleCount; a++)
    {
        long i1 = indices[a*3];
        long i2 = indices[a*3 + 1];
        long i3 = indices[a*3 + 2];
        
        auto v1 = vertex[i1];
        auto v2 = vertex[i2];
        auto v3 = vertex[i3];
        
        auto w1 = texcoord[i1];
        auto w2 = texcoord[i2];
        auto w3 = texcoord[i3];
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        GeoVector sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                       (t2 * z1 - t1 * z2) * r);
        GeoVector tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                       (s1 * z2 - s2 * z1) * r);
        
        
        int i1_sign = normal[i1].z > 0 ? -1 : 1;
        int i2_sign = normal[i2].z > 0 ? -1 : 1;
        int i3_sign = normal[i3].z > 0 ? -1 : 1;
        
        int sum = i1_sign + i2_sign + i3_sign;
        if( sum != 3 && sum != -3 ){
            auto i1_tangent = normal[i1].Cross( GeoVector(0,1,0) );
            auto i2_tangent = normal[i2].Cross( GeoVector(0,1,0) );
            auto i3_tangent = normal[i3].Cross( GeoVector(0,1,0) );
            
            auto i1_bitangent = normal[i1].Cross( i1_tangent );
            auto i2_bitangent = normal[i1].Cross( i2_tangent );
            auto i3_bitangent = normal[i1].Cross( i3_tangent );
            
            tangents[i1] += i1_tangent;
            tangents[i2] += i2_tangent;
            tangents[i3] += i3_tangent;
            
            bitangents[i1] += i1_bitangent;
            bitangents[i2] += i2_bitangent;
            bitangents[i3] += i3_bitangent;
            
            
            
        } else {
            tangents[i1] += sdir * i1_sign;
            tangents[i2] += sdir * i2_sign;
            tangents[i3] += sdir * i3_sign;
            
            bitangents[i1] += tdir * i1_sign;
            bitangents[i2] += tdir * i2_sign;
            bitangents[i3] += tdir * i3_sign;
        }
        
        
        
        
        
        
    }
    
    for (long a = 0; a < vertexCount; a++)
    {
        auto n = normal[a];
        auto t = tangents[a];
        
        // Gram-Schmidt orthogonalize
        tangents[a] = (t - n * n.Dot(t)).Normalize();
        
        // Calculate handedness
        tangents[a].w = n.Cross(t).Dot(bitangents[a]) < 0.0F ? -1.0F : 1.0F;
    }
    
    return std::pair< std::vector<GeoVector>, std::vector<GeoVector> >( tangents, bitangents );
}



std::pair< std::vector<GeoVector>, std::vector<GeoVector> > CalculateTangentArray(
                                                                                  std::vector<GeoFloat3> vertex,
                                                                                  std::vector<GeoVector> normal,
                                                                                  std::vector<GeoFloat2> texcoord,
                                                                                  std::vector<unsigned int> indices
                                                                                  )
{
    auto vertexCount = vertex.size();
    auto triangleCount = indices.size() / 3;
    
    std::vector<GeoVector> tangents, bitangents;
    tangents.resize( vertexCount );
    bitangents.resize( vertexCount );
    
    for (long a = 0; a < triangleCount; a++)
    {
        long i1 = indices[a*3];
        long i2 = indices[a*3 + 1];
        long i3 = indices[a*3 + 2];
        
        auto v1 = vertex[i1];
        auto v2 = vertex[i2];
        auto v3 = vertex[i3];
        
        auto w1 = texcoord[i1];
        auto w2 = texcoord[i2];
        auto w3 = texcoord[i3];
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        GeoVector sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                      (t2 * z1 - t1 * z2) * r);
        GeoVector tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                      (s1 * z2 - s2 * z1) * r);
        
        
        tangents[i1] += sdir;
        tangents[i2] += sdir;
        tangents[i3] += sdir;
            
        bitangents[i1] += tdir;
        bitangents[i2] += tdir;
        bitangents[i3] += tdir;
    }
    
    for (long a = 0; a < vertexCount; a++)
    {
        auto n = normal[a];
        auto t = tangents[a];
        
        // Gram-Schmidt orthogonalize
        tangents[a] = (t - n * n.Dot(t)).Normalize();
        
        // Calculate handedness
        tangents[a].w = n.Cross(t).Dot(bitangents[a]) < 0.0F ? -1.0F : 1.0F;
        
        bitangents[a] *= tangents[a].w;
        
    }
    
    
    
    
    
    return std::pair< std::vector<GeoVector>, std::vector<GeoVector> >( tangents, bitangents );
}



std::vector<Vertex> CalculateTangentArray( const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices ){
    auto decomposed_vertices = DecomposeVertices(vertices);
    auto positions = decomposed_vertices.first;
    auto normals = decomposed_vertices.second;
    auto texcoords = decomposed_vertices.third;
    
    auto tangent_bitangent_pair = CalculateTangentArray( positions, normals, texcoords, indices );
    auto tangents = tangent_bitangent_pair.first;
    auto bitangents = tangent_bitangent_pair.second;
    
    std::vector<Vertex> tangentized_vertices = vertices;
    for( int i = 0; i < vertices.size(); i++ ){
        tangentized_vertices[i].tangent = tangents[i].ToGeoFloat4();
        tangentized_vertices[i].bitangent = bitangents[i].ToGeoFloat3();
    }
    
    return tangentized_vertices;
}




void CalculateTangentArray( std::vector<Vertex>& vertices )
                           /*
                           
                           long vertexCount, const Point3D *vertex, const Vector3D *normal,
                           const Point2D *texcoord, long triangleCount, const Triangle *triangle, Vector4D *tangent) */
{
    for (long a = 0; a < vertices.size(); a += 3)
    {
        auto v1 = vertices[a].position;
        auto v2 = vertices[a+1].position;
        auto v3 = vertices[a+2].position;
        
        auto w1 = vertices[a].colorUV;
        auto w2 = vertices[a+1].colorUV;
        auto w3 = vertices[a+2].colorUV;
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        GeoVector sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                      (t2 * z1 - t1 * z2) * r);
        GeoVector tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                      (s1 * z2 - s2 * z1) * r);
        
        sdir = sdir.Normalize();
        tdir = tdir.Normalize();
        
        
        if( GeoVector(vertices[a].normal).Cross( sdir).Dot(tdir) < 0 ){
            sdir *= -1;
        }
        
        
        //if (glm::dot(glm::cross(n, t), b) < 0.0f){
        //    t = t * -1.0f;
        //}
        
        
        
        
  
        //https://github.com/mlimper/tgen/blob/master/src/tgen.cpp
      
        
        // can be used for tangent space to object space
        vertices[a].tangent = sdir.ToGeoFloat4();
        vertices[a].bitangent = tdir.ToGeoFloat3();
        
        vertices[a+1].tangent = sdir.ToGeoFloat4();
        vertices[a+1].bitangent = tdir.ToGeoFloat3();
        
        vertices[a+2].tangent = sdir.ToGeoFloat4();
        vertices[a+2].bitangent = tdir.ToGeoFloat3();
    }
    
    
    // use the following to get an object space to tangent space transform
    /*
    for (long a = 0; a < vertexCount; a++)
    {
        const Vector3D& n = normal[a];
        const Vector3D& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        tangent[a] = (t - n * Dot(n, t)).Normalize();
        
        // Calculate handedness
        tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    } */
    
}