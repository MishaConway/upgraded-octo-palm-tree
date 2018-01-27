#include "device.h"

OpenGL::StateManager OpenGL::GraphicsDevice::state_manager;
OpenGL::Capabilities OpenGL::GraphicsDevice::capabilities;
OpenGL::Viewport OpenGL::GraphicsDevice::viewport;

bool OpenGL::GraphicsDevice::initialized = false;




bool OpenGL::GraphicsDevice::Initialize( const unsigned int width, const unsigned int height )
{
    if( initialized )
        return true;
    
    if( GLEW_OK != glewInit() )
        return false;
    
    //capabilities.ForceDisablePixelBufferObject();
        
    SetViewport( width, height );
    //pBoundVertexBuffer = nullptr;
    
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
}

OpenGL::Viewport OpenGL::GraphicsDevice::GetViewport()
{
    return viewport;
}


OpenGL::Capabilities& OpenGL::GraphicsDevice::GetCapabilities()
{
    return capabilities;
}

void OpenGL::GraphicsDevice::Clear( Color c )
{
    glClearColor( c.GetNormalizedRed(), c.GetNormalizedGreen(), c.GetNormalizedBlue(), c.GetNormalizedAlpha() );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

