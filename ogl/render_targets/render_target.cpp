#include "render_target.h"

OpenGL::RenderTarget::RenderTarget()
{
    valid = false;
}

OpenGL::RenderTarget::RenderTarget( const unsigned int width, const unsigned int height )
{
    tex = Texture( width, height, TEXTURE_USAGE::RENDER_TARGET );
    //tex.SetTiling( 1, -1 );
    
    //generate and bind an opengl frame buffer object for this texture
    glGenFramebuffers( 1, &fbo_id );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );
    
    //attach the texture object to this framebuffer object
    printf( "render target texture id is %i\n", tex.GetOpenGLTextureId() );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.GetOpenGLTextureId(), 0 );
    
    glGenRenderbuffers(1, &depth_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, DEFAULT_DEPTH_BUFFER_FORMAT, width, height);
    
    //-------------------------
    //Attach depth buffer to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status)
    {
        case GL_FRAMEBUFFER_COMPLETE:
            printf("framebuffer complete\n");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            printf("framebuffer unsupported\n");
            break;
        case GL_FRAMEBUFFER_UNDEFINED:
            printf("framebuffer undefined\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            printf("framebuffer incomplete attachment\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            printf( "framebuffer incomplete missing attachment\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            printf( "framebuffer incomplete draw buffer\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            printf( "framebuffer incomplete read buffer\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            printf( "framebuffer incomplete multisample\n");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            printf( "framebuffer incomplete layer targets\n");
            break;
        case 0:
            printf( "framebuffer unknown error\n");
            break;
        default:
            printf("FORGET IT!\n");
            break;
    }
    
    //now that a framebuffer object has been created and attached to this texture, we can unbind it
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void OpenGL::RenderTarget::Free()
{
    
}

OpenGL::Texture& OpenGL::RenderTarget::GetTexture()
{
    return tex;
}

unsigned int OpenGL::RenderTarget::GetWidth(){
    return tex.GetWidth();
}

unsigned int OpenGL::RenderTarget::GetHeight(){
    return tex.GetHeight();
}

GLuint OpenGL::RenderTarget::GetOpenGLFrameBufferId(){
    return fbo_id;
}

GLuint OpenGL::RenderTarget::GetOpenGLDepthBufferId(){
    return depth_buffer_id;
}