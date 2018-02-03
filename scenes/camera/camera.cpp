#include "Camera.h"

#include <math.h>

Camera::Camera(){
    up = GeoVector(0,1,0);
    forward = GeoVector(0,0,-1);
}
Camera::~Camera(){}

void Camera::SyncViewTransform(){
    view_transform = GeoMatrix::LookAtRH( eye_position, focus_position, up );
}

void Camera::SetProjection( const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ){
    this->width = width;
    this->height = height;
    this->fovy = fovy;
    this->near_z = near_z;
    this->far_z = far_z;
    projection_transform = GeoMatrix::MatrixPerspectiveFovRH( GeoConvertToRadians(fovy),(float)width / (float)height, near_z, far_z );
}

void Camera::SetWidthHeight( const unsigned int width, const unsigned int height )
{
    this->width = width;
    this->height = height;
    projection_transform = GeoMatrix::MatrixPerspectiveFovRH( GeoConvertToRadians(fovy),(float)width / (float)height, near_z, far_z );
}

unsigned int Camera::GetWidth()
{
    return width;
}

unsigned int Camera::GetHeight()
{
    return height;
}


void Camera::SetEyePosition( const GeoVector& eye_position )
{
    this->eye_position = eye_position;
    SyncViewTransform();
}

void Camera::SetFocusPosition( const GeoVector& focus_position )
{
    this->focus_position = focus_position;
    SyncViewTransform();
}

GeoMatrix Camera::GetProjectionTransform()
{
    return projection_transform;
}

GeoMatrix Camera::GetViewTransform()
{
    return view_transform;
}

GeoMatrix Camera::GetReflectedViewTransform( const GeoVector& reflection_plane )
{
    return GeoMatrix::Reflection( reflection_plane ) * GetViewTransform();
}

GeoMatrix Camera::GetConstrainedBillboardTransform( const GeoVector& world_position ){
    return GeoMatrix::CreateConstrainedBillboard( world_position, GetEyePosition(), up, GetEyeDirectionNormalized(), forward );
}


GeoVector Camera::GetEyePosition()
{
    return eye_position;
}

GeoVector Camera::GetFocusPosition()
{
    return focus_position;
}

GeoVector Camera::GetEyeDirection()
{
    return GetFocusPosition() - GetEyePosition();
}

GeoVector Camera::GetEyeDirectionNormalized()
{
    return GetEyeDirection().Normalize();
}

GeoVector Camera::ProjectIntoScreenspace( const GeoVector& worldspace_vertex )
{
    const float    HalfViewportWidth = width * 0.5f;
    const float    HalfViewportHeight = height * 0.5f;
    GeoVector Scale(HalfViewportWidth,   -HalfViewportHeight,  1,  0.0f);
    GeoVector Offset(HalfViewportWidth, HalfViewportHeight, 0, 0.0f);
    return ( view_transform * projection_transform ).TransformCoord( GeoVector(worldspace_vertex, 1 )) * Scale + Offset;
}

GeoFloat2 Camera::ScreenspaceToClipspace( const GeoFloat2& screenspace_vertex )
{
    return GeoFloat2( screenspace_vertex.x / (width * 0.5f) - 1.0f, 1.0f- screenspace_vertex.y /(height*0.5f) );
}

float Camera::DistanceFromEye( const GeoVector& point )
{
    return (point - eye_position).Length();
}
