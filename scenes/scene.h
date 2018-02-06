#pragma once


#include "shader_cache/shader_cache.h"
#include "texture_cache/texture_cache.h"

#include "camera/camera.h"
#include "camera/first_person_camera.h"
#include "../ogl/buffers/vertex_buffers/vertex_buffer.h"
#include "../ogl/render_targets/render_target.h"

#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"
#include "../shapes/sphere.h"
#include "../shapes/cylinder.h"

#include "scene_graph/nodes/node.h"



//  1) render to back buffer
//  2) render to render target

// options...   enforce global shader

struct RenderPass{
    
    
    
};



class Scene{
public:
    void Initialize( const unsigned int width, const unsigned int height );
    void Draw();
    void Update( const float elapsed_seconds );
    
    Camera* GetCamera();
    
protected:
    void ConfigureShaderProgram( SceneGraph::Node* node, SceneGraph::IDrawable* drawable  );
    
    
    void UpdateNodes( SceneGraph::Node* node, GeoMatrix transform, const float elapsed_seconds );
    void DrawNodes( SceneGraph::Node* node );
    
    
    
    
protected:
    TextureCache texture_cache;
    ShaderCache shader_cache;
    
    OpenGL::RenderTarget render_target;
    
    std::vector<SceneGraph::LightNode*> light_nodes;
    
    
    
    Camera* camera;
    
    SceneGraph::Node* root;
};