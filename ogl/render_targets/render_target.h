#pragma once

#include "../textures/texture.h"
#include <vector>


namespace OpenGL{
    enum RENDER_TARGET_USAGE
    {
        RENDER_TARGET_MSAA = 1 << 0,
        RENDER_TARGET_ATTACH_TEXTURE = 1 << 1,
        RENDER_TARGET_FLOAT = 1 << 2,
        RENDER_TARGET_CUBEMAP = 1 << 3,
        RENDER_TARGET_ATTACH_DEPTH_BUFFER = 1 << 3

    };
    typedef uint8_t RenderTargetUsageMask;

    
    
    class RenderTarget : public IValidatable, IHasErrors
    {
    public:
        RenderTarget();
        RenderTarget( const unsigned int width, const unsigned int height );
        RenderTarget( const unsigned int width, const unsigned int height, const RenderTargetUsageMask usage );


        bool EnableMultisampling();
        bool EnableFloatMultisampling();
        bool AttachFloatTexture();
        bool AttachTexture();
        bool AttachFloatCubeMap();
        bool AttachCubeMap();
        bool AttachDepthBuffer();
        
        bool AttachCubemapSide( const CUBEMAP_SIDE side );
        
        
        GLuint GetOpenGLFrameBufferId() const;
        GLuint GetOpenGLDepthBufferId();
        
        void BlitToScreen(const unsigned int screen_width, const unsigned int screen_height);
        void BlitToRenderTarget( const RenderTarget& render_target );
        
        void Bind();
        
        Texture& GetTexture();
        void Free();
        
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
        
        bool CheckStatus();
    private:
        void Setup( const unsigned int width, const unsigned int height );
        bool AttachTexture( const TextureUsageMask texture_usage);
        bool EnableMultisampling( const bool use_float_format );
        RenderTargetUsageMask render_target_usage;


        
        unsigned int width, height;
        
        
        Texture tex;
        GLuint fbo_id, depth_buffer_id;
    };
}



