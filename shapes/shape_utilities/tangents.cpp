#include "tangents.h"


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
        GeoFloat3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                      (t2 * z1 - t1 * z2) * r);
        GeoFloat3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                      (s1 * z2 - s2 * z1) * r);
        
        
        
        // can be used for tangent space to object space
        vertices[a].tangent = sdir;
        vertices[a].bitangent = tdir;
        
        vertices[a+1].tangent = sdir;
        vertices[a+1].bitangent = tdir;
        
        vertices[a+2].tangent = sdir;
        vertices[a+2].bitangent = tdir;
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