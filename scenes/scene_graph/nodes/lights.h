#pragma once

#include "../../../geo/GeoMatrix.h"


namespace SceneGraph{
    struct Attenuation{
        float constant;
        float linear;
        float quadratic;
        Attenuation();
        GeoFloat3 ToGeoFloat3();
    };
    
    struct IBaseLightDetails{
        GeoFloat3 ambient;
        GeoFloat3 diffuse;
        GeoFloat3 specular;
        IBaseLightDetails();
    };
    
    struct IDirectionalLight : public IBaseLightDetails {
        bool directional;
        GeoVector direction;
        IDirectionalLight();
    };
    
    struct IPositionalLight : public IDirectionalLight {
        Attenuation attenuation;
    };
    
    struct ISpotlightDetails : public IPositionalLight {
        bool spotlight;
        float cone_angle;
        ISpotlightDetails();
    };
    
    struct IFullLightDetails : ISpotlightDetails{
        
    };
}