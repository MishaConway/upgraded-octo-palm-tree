#include "rounded_cube.h"


double mypower(double v,double n)
{
    if (v >= 0)
        return(pow(v,n));
    else
        return(-pow(-v,n));
}


RoundedCube::RoundedCube( const int meshResolution, const double cornerStrength ){
    GenerateQuads(meshResolution, cornerStrength);
}


void RoundedCube::GenerateQuads(  const int meshResolution, const double cornerStrength ){
    int i,j,index;
    int i1,i2,i3,i4;
    double theta,phi;
    std::vector<Vertex> p;

    
    int N = meshResolution; //64 mesh resolution
    double rpower = cornerStrength; //1  power that controls corner strength
    
    // Create vertices
    p.resize( (N+1)*(N/2+1) );
    
    
    // Pole is along the z axis
    for (j=0;j<=N/2;j++) {
        for (i=0;i<=N;i++) {
            index = j * (N+1) + i;
            theta = i * 2 * M_PI / N;
            phi = -0.5*M_PI + M_PI * j / (N/2.0);
            
            // Unit sphere, power determines roundness
            p[index].position.x = mypower(cos(phi),rpower) * mypower(cos(theta),rpower);
            p[index].position.y = mypower(cos(phi),rpower) * mypower(sin(theta),rpower);
            p[index].position.z = mypower(sin(phi),rpower);
            
            // Texture coordinates
            // Project texture coordinates from a surrounding sphere
            p[index].colorUV.x = atan2(p[index].position.y,p[index].position.x) / (2*M_PI);
            if (p[index].colorUV.x < 0)
                p[index].colorUV.x = 1 + p[index].colorUV.x;
            p[index].colorUV.y = 0.5 + atan2(p[index].position.z,sqrt(p[index].position.x*p[index].position.x+p[index].position.y*p[index].position.y)) / M_PI;
            
            // Seams
            if (j == 0) {
                p[index].position.x = 0;
                p[index].position.y = 0;
                p[index].position.z = -1;
            }
            if (j == N/2) {
                p[index].position.x = 0;
                p[index].position.y = 0;
                p[index].position.z = 1;
            }
            if (i == N) {
                p[index].position.x = p[j*(N+1)+i-N].position.x;
                p[index].position.y = p[j*(N+1)+i-N].position.y;
            }
            
        }
    }
    
    // create all normals
    for( int i = 0; i < p.size(); i++ ){
        p[i].normal = GeoVector(p[i].position).Normalize().ToGeoFloat3();
    }
    
    for (j=0;j<N/2;j++) {
        for (i=0;i<N;i++) {
            i1 =  j    * (N+1) + i;
            i2 =  j    * (N+1) + (i + 1);
            i3 = (j+1) * (N+1) + (i + 1);
            i4 = (j+1) * (N+1) + i;
            
            auto a = p[i1];
            auto b = p[i2];
            auto c = p[i3];
            auto d = p[i4];
            
            quads.push_back(Quad(a,b,c,d));
        }
    }
}


RoundedCube::~RoundedCube(){
    
}

RoundedCube RoundedCube::UnitRoundedCube(){
    return RoundedCube::UnitRoundedCube(64, 0.25f);
}

RoundedCube RoundedCube::UnitRoundedCube(const int meshResolution, const double cornerStrength ){
    return RoundedCube( meshResolution, cornerStrength);
}




