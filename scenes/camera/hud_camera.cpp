#include "hud_camera.h"

HudCamera::HudCamera(){
    
}

HudCamera::~HudCamera(){
    
}

void HudCamera::SetProjection2D( const unsigned int width, const unsigned int height){
    this->width = width;
    this->height = height;
    projection_transform = GeoMatrix::MatrixOrthoRH2D( width, height);
    view_transform = GeoMatrix::Identity();
}

void HudCamera::SetWidthHeight( const unsigned int width, const unsigned int height )
{
    SetProjection2D( width, height);
}


