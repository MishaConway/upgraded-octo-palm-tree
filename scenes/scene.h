#pragma once


#include "shader_cache/shader_cache.h"
#include "texture_cache/texture_cache.h"

#include "camera/camera.h"
#include "camera/first_person_camera.h"
#include "camera/hud_camera.h"
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
#include "scene_graph/renderers/pass.h"



class Scene{
public:
    void Initialize( const unsigned int width, const unsigned int height );
    void Draw();
    void Update( const float elapsed_seconds );
    
    Camera* GetCamera();
    
    int fudge, fudge2;

    
protected:
    void ConfigureShaderProgram( SceneGraph::Node* node, SceneGraph::IDrawable* drawable, Camera* cam, const std::string& override_shader_program  );
    
    
    void UpdateNodes( SceneGraph::Node* node, GeoMatrix transform, const float elapsed_seconds );
    void DrawNodesToRenderTarget( SceneGraph::Node* node, Camera* cam, OpenGL::RenderTarget render_target );
    void DrawNodesToRenderTarget( SceneGraph::Node* node, Camera* cam, OpenGL::RenderTarget render_target, const std::string& override_shader_program );

    
    void DrawNodesToScreen( SceneGraph::Node* node, Camera* cam );
    void DrawNodesToScreen( SceneGraph::Node* node, Camera* cam, const std::string& override_shader_program  );


    void DrawNodes( SceneGraph::Node* node, Camera* cam, const std::string& override_shader_program );

    
    
    
    
protected:
    unsigned int screen_width, screen_height;
    
    TextureCache texture_cache;
    ShaderCache shader_cache;
    
    OpenGL::RenderTarget render_target;
    
    std::vector<SceneGraph::LightNode*> light_nodes;
    
    Camera* camera;
    Camera* hud_camera;
    
    
    RenderChain render_chain;
    
    SceneGraph::Node* root;
    SceneGraph::Node* hud_root;
};