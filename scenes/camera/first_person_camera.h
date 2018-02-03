#pragma once

#include "camera.h"


class FirstPersonCamera : public Camera
{
public:
    FirstPersonCamera();
    virtual ~FirstPersonCamera();
    
    virtual void SetEyePosition( const GeoVector& eye_position );
    virtual void SetFocusPosition( const GeoVector& focus_position );
    
    
    void TurnLeft( const float degrees );
    void TurnRight( const float degrees );
    void LookUp( const float degrees );
    void LookDown( const float degrees );
    
    GeoVector GetWorldLeftVector();
    GeoVector GetWorldRightVector();
    

    void StrafeLeft( const float distance );
    void StrafeRight( const float distance );
    
    void MoveForward( const float distance );
    void MoveBackwards( const float distance );
    
    void MoveUp( const float distance );
    void MoveDown( const float distance );
    
    void Move( const GeoVector& vec );
    
    
protected:
    
    GeoMatrix GetLocalRotation( const bool use_vertical_rotation );
    
    
    void SyncEyePosition();
    void SyncFocusPosition();
    
protected:
    float horizontal_degrees, vertical_degrees;
    float max_vertical_degrees, min_vertical_degrees;

private:
    typedef Camera inherited;
    
 
};