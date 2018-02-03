#include "first_person_camera.h"
#include <cmath>

FirstPersonCamera::FirstPersonCamera(){
    horizontal_degrees = 0;
    vertical_degrees = 0;
    max_vertical_degrees = 30;
    min_vertical_degrees = -25;
}
FirstPersonCamera::~FirstPersonCamera(){}

void FirstPersonCamera::SetEyePosition( const GeoVector& eye_position ){
    inherited::SetEyePosition(eye_position);
    SyncFocusPosition();
}

void FirstPersonCamera::SetFocusPosition( const GeoVector& focus_position ){
    inherited::SetFocusPosition(focus_position);
    SyncEyePosition();
}

void FirstPersonCamera::TurnLeft( const float degrees ){
    horizontal_degrees = fmodf( horizontal_degrees + degrees, 360.0f );
    SyncFocusPosition();
}

void FirstPersonCamera::TurnRight( const float degrees ){
    TurnLeft( -degrees );
}

void FirstPersonCamera::LookUp( const float degrees ){
    vertical_degrees = fmodf( vertical_degrees + degrees, 360.0f );
    if( vertical_degrees > max_vertical_degrees )
        vertical_degrees = max_vertical_degrees;
    if( vertical_degrees < min_vertical_degrees )
        vertical_degrees = min_vertical_degrees;
    SyncFocusPosition();
}

void FirstPersonCamera::LookDown( const float degrees ){
    LookUp( -degrees );
}

GeoVector FirstPersonCamera::GetWorldLeftVector(){
    return -GetWorldRightVector();
}

GeoVector FirstPersonCamera::GetWorldRightVector(){
    return GetEyeDirection().ZeroY().Normalize().Cross( up );
}

void FirstPersonCamera::StrafeLeft( const float distance ){
    StrafeRight( -distance );
}

void FirstPersonCamera::StrafeRight( const float distance ){
    Move( forward.Cross(up) * distance );
}

void FirstPersonCamera::MoveForward( const float distance ){
    Move( forward * distance );    
}

void FirstPersonCamera::MoveBackwards( const float distance ){
    MoveForward( -distance );
}

void FirstPersonCamera::MoveUp( const float distance ){
    Move( up * distance );
}

void FirstPersonCamera::MoveDown( const float distance ){
    MoveUp( -distance );
}

void FirstPersonCamera::Move( const GeoVector& vec ){
    SetEyePosition( eye_position + GetLocalRotation(false) * vec );
}


void FirstPersonCamera::SyncEyePosition(){
    eye_position = focus_position - GetLocalRotation(false) * forward;
    SyncViewTransform();
}

void FirstPersonCamera::SyncFocusPosition(){
    focus_position = eye_position + GetLocalRotation(true) * forward;
    SyncViewTransform();
}

GeoMatrix FirstPersonCamera::GetLocalRotation( const bool use_vertical_rotation ){
    auto quat = GeoQuaternion( up, horizontal_degrees );
    if( use_vertical_rotation )
        quat *= GeoQuaternion( GeoVector(1,0,0), vertical_degrees );
    return quat.ToMatrix();
}
