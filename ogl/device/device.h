#pragma once

#include "glew.h"
#include <string>
#include "state_manager/state_manager.h"
#include "capabilities/capabilities.h"
#include "../buffers/vertex_buffers/vertex_buffer.h"
#include "../textures/texture.h"
#include "../render_targets/render_target.h"



namespace OpenGL{
    struct Viewport
    {
        float TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth;
    };
    
    
    class GraphicsDevice
    {
    public:
        static bool Initialize();
        
        static std::string GetOpenGLVersion();
        
        static bool Draw( const unsigned int num_vertices );
        
        static Viewport GetViewport();
        static void SetViewport( const unsigned int width, const unsigned int height );
        
        static StateManager& GetStateManager();
        static Capabilities& GetCapabilities();
        
        static void Clear( Color color );
        
        //void    SetVertexBuffer( OpenGLVertexBuffer& pVertexBuffer );
        //void    SetVertexBuffer( OpenGLDynamicVertexBuffer& pVertexBuffer );
        
        void SetDefaultRenderTarget();
        void SetRenderTarget( RenderTarget& render_target );
        void SetRenderTarget( RenderTarget& render_target, const Color& clear_color );
    private:
        static StateManager state_manager;
        static Capabilities capabilities;
        static Viewport viewport;
        static bool initialized;
    };
}

