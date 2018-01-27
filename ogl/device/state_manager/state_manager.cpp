#include "state_manager.h"
#include "glew.h"



OpenGL::StateManager::StateManager(){
    locked = false;
    blend_state_locked = false;
}


void OpenGL::StateManager::Lock()
{
    if( locked )
        throw "graphics device state manager is already locked!";
    locked = true;
}

void OpenGL::StateManager::Unlock()
{
    locked = false;
}

void OpenGL::StateManager::LockBlendState()
{
    if( blend_state_locked )
        throw "blend state already locked!";
    blend_state_locked = true;
}

void OpenGL::StateManager::UnlockBlendState()
{
    blend_state_locked = false;
}

void OpenGL::StateManager::SetDefaults()
{
    SetDefaultFrontFaceRendering();
   	SetDefaultBlendState();
}

void OpenGL::StateManager::SetBlendType( const STATE_MANAGER_BLEND_TYPE blend_type )
{
    switch( blend_type )
    {
        case STATE_MANAGER_BLEND_TYPE::NONE:
            SetDefaultBlendState();
            break;
        case STATE_MANAGER_BLEND_TYPE::ALPHA:
            EnableAlphaBlending();
            break;
        case STATE_MANAGER_BLEND_TYPE::ADDITIVE:
            EnableAdditiveBlending();
            break;
        default:
            throw "invalid blend type passed";
    }
}

void OpenGL::StateManager::SetDefaultFrontFaceRendering()
{
    if( locked )
        return;
    glFrontFace( GL_CW );
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glDepthMask( GL_TRUE );
}

void OpenGL::StateManager::SetDefaultBackFaceRendering()
{
    if( locked )
        return;
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glDepthMask( GL_TRUE );
}

void OpenGL::StateManager::SetDefaultFrontAndBackRendering()
{
    if( locked )
        return;
    glDisable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glDepthMask( GL_TRUE );
}

void OpenGL::StateManager::SetParticleRendering()
{
    if( locked )
        return;
    EnableAdditiveBlending();
    glDepthMask( GL_FALSE );
    glDisable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
}

void OpenGL::StateManager::SetSpriteRendering( const STATE_MANAGER_BLEND_TYPE blend_type )
{
    if( locked )
        return;
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    SetBlendType( blend_type );
}

void OpenGL::StateManager::EnableAlphaBlending()
{
    if( locked || blend_state_locked )
        return;
    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGL::StateManager::EnableAdditiveBlending()
{
    if( locked || blend_state_locked )
        return;
    glEnable( GL_BLEND );
    glBlendFunc(GL_ONE, GL_ONE);
}

void OpenGL::StateManager::DisableBlending()
{
    if( locked )
        return;
    SetDefaultBlendState();
}

void OpenGL::StateManager::SetDefaultBlendState()
{
    if( locked || blend_state_locked )
        return;
    glDisable( GL_BLEND );
}