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
        GeoFloat3 rim;
        float shininess;
        
        static Material Zero();
        static Material Emerald();
        static Material Jade();
        static Material Obsidian();
        static Material Pearl();
        static Material Ruby();
        static Material Turquoise();
        static Material Brass();
        static Material Bronze();
        static Material Chrome();
        static Material Copper();
        static Material Gold();
        static Material Silver();
        static Material BlackPlastic();
        static Material CyanPlastic();
        static Material GreenPlastic();
        static Material RedPlastic();
        static Material WhitePlastic();
        static Material YellowPlastic();
        static Material BlackRubber();
        static Material CyanRubber();
        static Material GreenRubber();
        static Material RedRubber();
        static Material WhiteRubber();
        static Material YellowRubber();
    };
}