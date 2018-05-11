#include "render_target.h"

OpenGL::RenderTarget::RenderTarget(){
    valid = false;
    fbo_id = 0;
    depth_buffer_id = 0;
    render_target_usage = 0;
}

OpenGL::RenderTarget::RenderTarget( const unsigned int width, const unsigned int height ){
    Setup( width, height );
}

OpenGL::RenderTarget::RenderTarget( const unsigned int width, const unsigned int height, const RenderTargetUsageMask usage ){
    Setup(width, height);
    render_target_usage = usage;
    const bool use_float_format = usage & RENDER_TARGET_USAGE::RENDER_TARGET_FLOAT;
    
    if( usage & RENDER_TARGET_USAGE::RENDER_TARGET_ATTACH_DEPTH_BUFFER )
        AttachDepthBuffer();
    
    if( usage & RENDER_TARGET_USAGE::RENDER_TARGET_MSAA ){
      EnableMultisampling( use_float_format );
    } else {
        if( usage & RENDER_TARGET_USAGE::RENDER_TARGET_ATTACH_TEXTURE ){
            if( usage & RENDER_TARGET_USAGE::RENDER_TARGET_CUBEMAP ){
                if( use_float_format )
                    AttachFloatCubeMap();
                else
                    AttachCubeMap();
            } else {
                if( use_float_format )
                    AttachFloatTexture();
                else
                    AttachTexture();
            }
        }
    }
    
    
}


bool OpenGL::RenderTarget::EnableMultisampling(){
    return EnableMultisampling(false);
}

bool OpenGL::RenderTarget::EnableFloatMultisampling(){
    return EnableMultisampling(true);
}

bool OpenGL::RenderTarget::EnableMultisampling(const bool use_float_format){
    GLint max_samples;
    glGetIntegerv(GL_MAX_SAMPLES, &max_samples);
    
    GLenum internal_format = GL_RGBA;
    if( use_float_format )
        internal_format = GL_RGBA16F;
    
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    
    GLuint m_ColorId;
    glGenRenderbuffers(1, &m_ColorId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_ColorId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, max_samples, internal_format, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorId);

    if( depth_buffer_id ){
        glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, max_samples, GL_DEPTH24_STENCIL8, width, height);
    }

    auto gl_error = glGetError();
    switch( gl_error ){
        case GL_NO_ERROR:
            printf( "no error\n");
            break;
        case GL_INVALID_ENUM:
            printf( "invalid enum\n");
            break;
        case GL_INVALID_VALUE:
            printf( "invalid value\n" );
            break;
        case GL_INVALID_OPERATION:
            printf( "invalid operation\n" );
            break;
        case GL_STACK_OVERFLOW:
            printf( "stack overflow\n" );
            break;
        case GL_STACK_UNDERFLOW:
            printf( "stack underflow\n" );
            break;
        case GL_OUT_OF_MEMORY:
            printf( "out of memory\n" );
            break;
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return CheckStatus();
}

bool OpenGL::RenderTarget::AttachTexture(){
    return AttachTexture( TEXTURE_USAGE_RENDER_TARGET );
}

bool OpenGL::RenderTarget::AttachFloatTexture(){
    return AttachTexture( TEXTURE_USAGE_RENDER_TARGET | TEXTURE_USAGE_FLOAT );;
}

bool OpenGL::RenderTarget::AttachCubeMap(){
    return AttachTexture( TEXTURE_USAGE_RENDER_TARGET | TEXTURE_USAGE_CUBEMAP );
}

bool OpenGL::RenderTarget::AttachFloatCubeMap(){
    return AttachTexture( TEXTURE_USAGE_RENDER_TARGET | TEXTURE_USAGE_FLOAT | TEXTURE_USAGE_CUBEMAP );
}

bool OpenGL::RenderTarget::AttachTexture( const TextureUsageMask texture_usage){
    tex = Texture( width, height, texture_usage );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    
    if( !(render_target_usage & RENDER_TARGET_USAGE::RENDER_TARGET_CUBEMAP) )
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.GetOpenGLTextureId(), 0 );
    
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return CheckStatus();
}

bool OpenGL::RenderTarget::AttachCubemapSide( const CUBEMAP_SIDE side ){
    GLenum opengl_cubemap_side = ToOpenGLCubeMapSide( side );
    
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, opengl_cubemap_side, tex.GetOpenGLTextureId(), 0);
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return CheckStatus();
}


bool OpenGL::RenderTarget::AttachDepthBuffer(){
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    glGenRenderbuffers(1, &depth_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return CheckStatus();
}

void OpenGL::RenderTarget::Setup( const unsigned int width, const unsigned int height ){
    render_target_usage = 0;
    this->width = width;
    this->height = height;
    glGenFramebuffers( 1, &fbo_id );
    depth_buffer_id = 0;
}

void OpenGL::RenderTarget::BlitToScreen(const unsigned int screen_width, const unsigned int screen_height){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id); // Make sure your multisampled FBO is the read framebuffer
    glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
    glBlitFramebuffer(0, 0, GetWidth(), GetHeight(), 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glBindRenderbuffer(GL_RENDERBUFFER, 0 );
}

void OpenGL::RenderTarget::BlitToRenderTarget( const RenderTarget& render_target ){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, render_target.GetOpenGLFrameBufferId() );
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_id);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, GetWidth(), GetHeight(), 0, 0, render_target.GetWidth(), render_target.GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glBindRenderbuffer(GL_RENDERBUFFER, 0 );
}

bool OpenGL::RenderTarget::CheckStatus(){
    valid = false;
    
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status)
    {
        case GL_FRAMEBUFFER_COMPLETE:
            valid = true;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            errors = "framebuffer unsupported\n";
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            errors = "framebuffer undefined\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            errors = "framebuffer incomplete attachment\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            errors = "framebuffer incomplete missing attachment\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            errors = "framebuffer incomplete draw buffer\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            errors = "framebuffer incomplete read buffer\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            errors = "framebuffer incomplete multisample\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            errors = "framebuffer incomplete layer targets\n";
            break;
        case 0:
            errors = "framebuffer unknown error\n";
            break;
        default:
            errors = "unknown error\n";
            break;
    }
    
    if( !valid )
        printf( "errors are %s \n", errors.c_str() );

    
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    
    return valid;
}




void OpenGL::RenderTarget::Free(){
    
}

OpenGL::Texture& OpenGL::RenderTarget::GetTexture(){
    return tex;
}

unsigned int OpenGL::RenderTarget::GetWidth() const{
    return width;
}

unsigned int OpenGL::RenderTarget::GetHeight() const{
    return height;
}

GLuint OpenGL::RenderTarget::GetOpenGLFrameBufferId() const{
    return fbo_id;
}

GLuint OpenGL::RenderTarget::GetOpenGLDepthBufferId(){
    return depth_buffer_id;
}