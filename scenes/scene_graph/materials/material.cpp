#include "material.h"


SceneGraph::Material::Material(){
    ambient = GeoFloat3( 0.05f, 0.05f, 0.05f );
    diffuse = GeoFloat3( 1, 1, 1 );
    specular = GeoFloat3( 1, 1, 1 );
    emissive = GeoFloat3( 0, 0, 0 );
    shininess = 128;
}

SceneGraph::Material SceneGraph::Material::Zero(){
    Material mat;
    mat.ambient  = GeoFloat3( 0,0,0 );
    mat.diffuse  = GeoFloat3( 0,0,0 );
    mat.specular = GeoFloat3( 0,0,0 );
    mat.shininess = 1;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Emerald(){
    Material mat;
    mat.ambient  = GeoFloat3( 0.0215f,  0.1745f,   0.0215f );
    mat.diffuse  = GeoFloat3( 0.07568f, 0.61424f,  0.07568f );
    mat.specular = GeoFloat3( 0.633f,	0.727811f, 0.633f );
    mat.shininess = 0.6f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Jade(){
    Material mat;
    mat.ambient = GeoFloat3(  0.135f,    0.2225f,   0.1575f );
    mat.diffuse = GeoFloat3(  0.54f,	 0.89f,	    0.63f );
    mat.specular = GeoFloat3( 0.316228f, 0.316228f,	0.316228 );
    mat.shininess = 0.1f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Obsidian(){
    Material mat;
    mat.ambient = GeoFloat3(  0.05375f,	 0.05f,     0.06625f );
    mat.diffuse = GeoFloat3(  0.18275f,	 0.17f,     0.22525f );
    mat.specular = GeoFloat3( 0.332741f, 0.328634f,	0.346435f );
    mat.shininess = 0.3f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Pearl(){
    Material mat;
    mat.ambient = GeoFloat3(  0.25f,	0.20725f,  0.20725f );
    mat.diffuse = GeoFloat3(  1.0f,     0.829f,    0.829f );
    mat.specular = GeoFloat3( 0.296648,	0.296648f, 0.296648f);
    mat.shininess = 0.088f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Ruby(){
    Material mat;
    mat.ambient = GeoFloat3( 0.1745f,	0.01175f,	0.01175f );
    mat.diffuse = GeoFloat3( 0.61424f,	0.04136f,	0.04136f );
    mat.specular = GeoFloat3( 0.727811f,	0.626959f,	0.626959f );
    mat.shininess = 0.6f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Turquoise(){
    Material mat;
    mat.ambient = GeoFloat3(0.1f,	0.18725f,	0.1745f );
    mat.diffuse = GeoFloat3(0.396f,	0.74151f,	0.69102f );
    mat.specular = GeoFloat3(0.297254f,	0.30829f,	0.306678f );
    mat.shininess = 0.1f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Brass(){
    Material mat;
    mat.ambient = GeoFloat3( 0.329412f,	0.223529f,	0.027451f);
    mat.diffuse = GeoFloat3( 0.780392f,	0.568627f,	0.113725f);
    mat.specular = GeoFloat3( 0.992157f,	0.941176f,	0.807843f);
    mat.shininess = 0.21794872f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Bronze(){
    Material mat;
    mat.ambient = GeoFloat3( 0.2125f,	0.1275f,	0.054f );
    mat.diffuse = GeoFloat3( 0.714f,	0.4284f,	0.18144f);
    mat.specular = GeoFloat3( 0.393548f,	0.271906f,	0.166721f );
    mat.shininess = 0.2f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Chrome(){
    Material mat;
    mat.ambient = GeoFloat3( 0.25f,	0.25f,	0.25f );
    mat.diffuse = GeoFloat3( 0.4f,	0.4f,	0.4f );
    mat.specular = GeoFloat3( 0.774597f,	0.774597f,	0.774597f );
    mat.shininess = 0.6f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Copper(){
    Material mat;
    mat.ambient = GeoFloat3( 0.19125f,	0.0735f,	0.0225f	);
    mat.diffuse = GeoFloat3( 0.7038f,	0.27048f,	0.0828f );
    mat.specular = GeoFloat3( 0.256777f,	0.137622f,	0.086014f );
    mat.shininess = 0.1f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Gold(){
    Material mat;
    mat.ambient = GeoFloat3( 0.24725f,	0.1995f,	0.0745f );
    mat.diffuse = GeoFloat3( 0.75164f,	0.60648f,	0.22648f );
    mat.specular = GeoFloat3( 0.628281f,	0.555802f,	0.366065f );
    mat.shininess = 0.4f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::Silver(){
    Material mat;
    mat.ambient = GeoFloat3( 0.19225f,	0.19225f,	0.19225f );
    mat.diffuse = GeoFloat3( 0.50754f,	0.50754f,	0.50754f );
    mat.specular = GeoFloat3( 0.508273f,	0.508273f,	0.508273f );
    mat.shininess = 0.4f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::BlackPlastic(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.0f,	0.0f );
    mat.diffuse = GeoFloat3( 0.01f,	0.01f,	0.01f );
    mat.specular = GeoFloat3( 0.50f,	0.50f,	0.50f );
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::CyanPlastic(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.1f,	0.06f );
    mat.diffuse = GeoFloat3( 0.0f,	0.50980392f,	0.50980392f );
    mat.specular = GeoFloat3( 0.50196078f,	0.50196078f,	0.50196078f );
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::GreenPlastic(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.0f,	0.0f );
    mat.diffuse = GeoFloat3( 0.1f,	0.35f,	0.1f );
    mat.specular = GeoFloat3( 0.45f,	0.55f,	0.45f);
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::RedPlastic(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.0f,	0.0f );
    mat.diffuse = GeoFloat3( 0.5f,	0.0f,	0.0f );
    mat.specular = GeoFloat3( 0.7f,	0.6f,	0.6f );
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::WhitePlastic(){
    Material mat;
    mat.ambient = GeoFloat3(0.0f,	0.0f,	0.0f );
    mat.diffuse = GeoFloat3( 0.55f,	0.55f,	0.55f );
    mat.specular = GeoFloat3( 0.70f,	0.70f,	0.70f );
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::YellowPlastic(){
    Material mat;
    mat.ambient = GeoFloat3(  0.0f,	 0.0f,	0.0f );
    mat.diffuse = GeoFloat3(  0.5f,	 0.5f,	0.0f );
    mat.specular = GeoFloat3( 0.60f, 0.60f,	0.50f );
    mat.shininess = .25f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::BlackRubber(){
    Material mat;
    mat.ambient = GeoFloat3( 0.02f,	0.02f,	0.02f );
    mat.diffuse = GeoFloat3( 0.01f,	0.01f,	0.01f );
    mat.specular = GeoFloat3( 0.4f,	0.4f,	0.4f );
    mat.shininess = .078125f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::CyanRubber(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.05f,	0.05f );
    mat.diffuse = GeoFloat3( 0.4f,	0.5f,	0.5f );
    mat.specular = GeoFloat3( 0.04f,	0.7f,	0.7f );
    mat.shininess = .078125f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::GreenRubber(){
    Material mat;
    mat.ambient = GeoFloat3( 0.0f,	0.05f,	0.0f );
    mat.diffuse = GeoFloat3( 0.4f,	0.5f,	0.4f );
    mat.specular = GeoFloat3( 0.04f,	0.7f,	0.04f );
    mat.shininess = .078125f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::RedRubber(){
    Material mat;
    mat.ambient = GeoFloat3(0.05f,	0.0f,	0.0f );
    mat.diffuse = GeoFloat3( 0.5f,	0.4f,	0.4f);
    mat.specular = GeoFloat3(0.7f,	0.04f,	0.04f );
    mat.shininess = .078125f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::WhiteRubber(){
    Material mat;
    mat.ambient = GeoFloat3( 0.05f,	0.05f,	0.05f);
    mat.diffuse = GeoFloat3( 0.5f,	0.5f,	0.5f);
    mat.specular = GeoFloat3( 0.7f,	0.7f,	0.7f);
    mat.shininess = .078125f * 128.0f;
    return mat;
}

SceneGraph::Material SceneGraph::Material::YellowRubber(){
    Material mat;
    mat.ambient = GeoFloat3( 0.05f,	0.05f,	0.0	);
    mat.diffuse = GeoFloat3( 0.5f,	0.5f,	0.4f);
    mat.specular = GeoFloat3( 0.7f,	0.7f,	0.04f);
    mat.shininess = .078125f * 128.0f;
    return mat;
}
