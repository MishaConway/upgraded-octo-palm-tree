#pragma once

#include "../../geo/GeoMatrix.h"
#include "../../geo/GeoQuaternion.h"

class Camera
{
public:
    Camera();
    virtual ~Camera();
    
    void SetProjection( const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z );
    
    unsigned int GetWidth();
    unsigned int GetHeight();
    void SetWidthHeight( const unsigned int width, const unsigned int height );
    
    virtual void SetEyePosition( const GeoVector& eye_position );
    virtual void SetFocusPosition( const GeoVector& focus_position );
    
    GeoMatrix GetProjectionTransform();
    GeoMatrix GetViewTransform();
    GeoMatrix GetReflectedViewTransform( const GeoVector& reflection_plane );
    GeoMatrix GetConstrainedBillboardTransform( const GeoVector& world_position );
    
    
    GeoVector GetEyePosition();
    GeoVector GetFocusPosition();
    GeoVector GetEyeDirection();
    GeoVector GetEyeDirectionNormalized();
    
    GeoFloat2 ScreenspaceToClipspace( const GeoFloat2& screensapce_vertex );
    
    GeoVector ProjectIntoScreenspace( const GeoVector& worldspace_vertex );
    
    float DistanceFromEye( const GeoVector& point );
    
protected:
    void SyncViewTransform();
    
protected:
    GeoMatrix projection_transform;
    unsigned int width;
    unsigned int height;
    float fovy;
    float near_z;
    float far_z;
    
    GeoMatrix view_transform;
    GeoVector eye_position, focus_position, up, forward;
};