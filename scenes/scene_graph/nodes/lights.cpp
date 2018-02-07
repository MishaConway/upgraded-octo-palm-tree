#include "lights.h"

SceneGraph::Attenuation::Attenuation(){
    this->constant = 1;
    this->linear = 0;
    this->quadratic = 0;
}

GeoFloat3 SceneGraph::Attenuation::ToGeoFloat3(){
    return GeoFloat3( this->constant, linear, quadratic );
}


SceneGraph::IBaseLightDetails::IBaseLightDetails(){
    ambient = GeoFloat3(0,0,0);
    diffuse = GeoFloat3(1,1,1);
    specular = GeoFloat3(1,1,1);
}

SceneGraph::IDirectionalLight::IDirectionalLight(){
    directional = false;
}

SceneGraph::ISpotlightDetails::ISpotlightDetails(){
    spotlight = false;
    cone_angle = 0;
}



