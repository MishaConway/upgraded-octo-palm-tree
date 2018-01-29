#include "material.h"


SceneGraph::Material::Material(){
    ambient = GeoFloat3( 0.05f, 0.05f, 0.05f );
    diffuse = GeoFloat3( 1, 1, 1 );
    specular = GeoFloat3( 0, 0, 0 );
    emissive = GeoFloat3( 0, 0, 0 );
    float shininess;
    
}