#pragma once

#include "../textures/texture.h"
#include <vector>


namespace OpenGL{
    class RenderTarget : public IValidatable, IHasErrors
    {
    public:
        RenderTarget();
        RenderTarget( const unsigned int width, const unsigned int height );
        RenderTarget( const unsigned int width, const unsigned int height, const bool use_float_format );

        bool EnableMultisampling();
        bool AttachTexture();
        
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
        void Setup( const unsigned int width, const unsigned int height, const bool use_float_format );
        
        unsigned int width, height;
        bool use_float_format;
        
        
        Texture tex;
        GLuint fbo_id, depth_buffer_id;
    };
}



