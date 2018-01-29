#pragma once


#include "shader_cache/shader_cache.h"
#include "texture_cache/texture_cache.h"

#include "camera/camera.h"
#include "../ogl/buffers/vertex_buffers/vertex_buffer.h"

#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"
#include "../shapes/sphere.h"

#include "scene_graph/node.h"




class Scene{
public:
    void Initialize( const unsigned int width, const unsigned int height );
    void Draw();
    void Update( unsigned int elapsed_milliseconds );
    
protected:
    void ConfigureShaderProgram( SceneGraph::Geode* geode );
    
    void TraverseNodes( GeoMatrix transform, SceneGraph::Node* node );
    
    
    
    
protected:
    TextureCache texture_cache;
    ShaderCache shader_cache;
    
    
    
    Camera camera;
    
    SceneGraph::Node* root;
};