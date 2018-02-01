#include "../../../geo/GeoFloat.h"

// http://devernay.free.fr/cours/opengl/materials.html
// http://kylehalladay.com/blog/tutorial/2014/02/18/Fresnel-Shaders-From-The-Ground-Up.html

// http://in2gpu.com/2014/06/19/lighting-vertex-fragment-shader/

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