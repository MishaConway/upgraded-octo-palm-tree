#pragma once

#include "camera.h"


class HudCamera : public Camera
{
public:
    HudCamera();
    HudCamera( const unsigned int width, const unsigned int height );
    virtual ~HudCamera();
    
    void SetProjection2D( const unsigned int width, const unsigned int height);
    virtual void SetWidthHeight( const unsigned int width, const unsigned int height );
private:
    typedef Camera inherited;

};