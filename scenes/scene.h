#pragma once


#include "shader_cache/shader_cache.h"
#include "camera/camera.h"
#include "../ogl/buffers/vertex_buffers/vertex_buffer.h"
#include "../ogl/textures/texture.h"

#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"
#include "../shapes/sphere.h"




class Scene{
public:
    void Initialize();
    void Draw();
    
protected:
    void ConfigureShaderProgram( OpenGL::VertexBuffer<Vertex>& vertex_buffer );
    
    
    
    
protected:
    
    ShaderCache shader_cache;
    Camera camera;
    OpenGL::VertexBuffer<Vertex> vertex_buffer;
    OpenGL::Texture texture;
    
    
    
    
};