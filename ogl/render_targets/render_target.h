#pragma once

#include "../textures/texture.h"
#include <vector>

#ifdef GL_ES_VERSION_2_0
#define DEFAULT_DEPTH_BUFFER_FORMAT GL_DEPTH_COMPONENT16
#else
#define DEFAULT_DEPTH_BUFFER_FORMAT GL_DEPTH_COMPONENT24
#endif


namespace OpenGL{
    class RenderTarget : public IValidatable
    {
    public:
        RenderTarget();
        RenderTarget( const unsigned int width, const unsigned int height );
        
        GLuint GetOpenGLFrameBufferId();
        GLuint GetOpenGLDepthBufferId();
        
        void Bind();
        
        Texture& GetTexture();
        void Free();
    private:
        Texture tex;
        GLuint fbo_id, depth_buffer_id;
    };
}



