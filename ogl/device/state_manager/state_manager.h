#pragma once

namespace OpenGL{
    enum STATE_MANAGER_BLEND_TYPE
    {
        NONE, ALPHA, ADDITIVE
    };
    
    
    
    class StateManager
    {
    public:
        StateManager();
        virtual void SetDefaults();
        
        virtual void SetDefaultFrontFaceRendering();
        virtual void SetDefaultBackFaceRendering();
        virtual void SetDefaultFrontAndBackRendering();
        
        virtual void SetBlendType( const STATE_MANAGER_BLEND_TYPE blend_type );
        virtual void SetParticleRendering();
        virtual void SetSpriteRendering( const STATE_MANAGER_BLEND_TYPE blend_type = STATE_MANAGER_BLEND_TYPE::NONE );
        
        virtual void EnableAlphaBlending();
        virtual void EnableAdditiveBlending();
        virtual void DisableBlending();
        
        virtual void Lock();
        virtual void Unlock();
        
        virtual void LockBlendState();
        virtual void UnlockBlendState();
    protected:
        virtual void SetDefaultBlendState();
    protected:
        bool locked, blend_state_locked;
    };
}
