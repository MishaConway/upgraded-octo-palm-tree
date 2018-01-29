#include "../../../geo/GeoFloat.h"



namespace SceneGraph{
    
    struct Material{
        Material();
        
        GeoFloat3 ambient;
        GeoFloat3 diffuse;
        GeoFloat3 specular;
        GeoFloat3 emissive;
        float shininess;
    };
}