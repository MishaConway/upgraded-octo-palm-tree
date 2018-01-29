#inclue "../../geo/GeoFloat.h"



namespace SceneGraph{
    
    class Material{
        GeoFloat3 ambient;
        GeoFloat3 diffuse;
        GeoFloat3 specular;
        GeoFloat3 emissive;
        float shininess;
    };
}