#include "device.h"

OpenGL::StateManager OpenGL::GraphicsDevice::state_manager;
OpenGL::Capabilities OpenGL::GraphicsDevice::capabilities;
OpenGL::Viewport OpenGL::GraphicsDevice::viewport;

bool OpenGL::GraphicsDevice::initialized = false;




bool OpenGL::GraphicsDevice::Initialize()
{
    if( initialized )
        return true;
    
    if( GLEW_OK != glewInit() )
        return false;
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    GetStateManager().SetDefaultFrontFaceRendering();
    //GetStateManager().SetDefaultFrontAndBackRendering();
    
    initialized = true;
    return true;
}

std::string OpenGL::GraphicsDevice::GetOpenGLVersion(){
    return std::string( (char*) glGetString( GL_VERSION ) );
}


void OpenGL::GraphicsDevice::SetViewport( const unsigned int width, const unsigned int height )
{
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    glViewport(0, 0, GLsizei(width), GLsizei(height));
}

OpenGL::Viewport OpenGL::GraphicsDevice::GetViewport()
{
    return viewport;
}


OpenGL::Capabilities& OpenGL::GraphicsDevice::GetCapabilities()
{
    return capabilities;
}

OpenGL::StateManager& OpenGL::GraphicsDevice::GetStateManager(){
    return state_manager;
}

void OpenGL::GraphicsDevice::Clear( Color c )
{
    glClearColor( c.GetNormalizedRed(), c.GetNormalizedGreen(), c.GetNormalizedBlue(), c.GetNormalizedAlpha() );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void OpenGL::GraphicsDevice::SetDefaultRenderTarget()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glBindRenderbuffer(GL_RENDERBUFFER, 0 );
}

void OpenGL::GraphicsDevice::SetRenderTarget( RenderTarget& render_target )
{
    glBindFramebuffer( GL_FRAMEBUFFER, render_target.GetOpenGLFrameBufferId() );
    glBindRenderbuffer(GL_RENDERBUFFER, render_target.GetOpenGLDepthBufferId() );
}

void OpenGL::GraphicsDevice::SetRenderTarget( RenderTarget& render_target, const Color& clear_color )
{
    SetRenderTarget( render_target );
    Clear( clear_color );
}

