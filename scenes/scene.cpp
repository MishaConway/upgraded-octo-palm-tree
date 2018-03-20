#include "scene.h"
#include "../ogl/device/device.h"
#include "../string/string_utils.h"
#include "../shapes/shape_utilities/debug_lines.h"

//https://www.keithlantz.net/2011/10/tangent-space-normal-mapping-with-glsl/


//https://graphics.stanford.edu/~mdfisher/cloth.html

// http://www.alkemi-games.com/a-game-of-tricks/

// http://andrew.wang-hoyer.com/experiments/cloth/

void Scene::Initialize( const unsigned int width, const unsigned int height ){
    screen_width = width;
    screen_height = height;
    
    
    unit_quad_vertex_buffer = OpenGL::VertexBuffer<Vertex>(Quad::XYUnitQuad().Transform( GeoMatrix::Scaling(2)).ToVertices());
    
    
    fudge = 0;
    
    
    camera = new FirstPersonCamera();
    camera->SetProjection( width, height, 45.0f, 0.1f, 100.0f );
    camera->SetEyePosition(GeoVector(-1, 0.5f, 4 ));
    
    hud_camera = new HudCamera();
    hud_camera->SetWidthHeight( width, height );
    
    // todo: consider replacing so that different programs get their own global textures
    global_textures["ramp"] = SceneGraph::TextureDetails( "ramp.png" );
    

    
    shader_cache.RegisterShaderProgram( "hud" );
    shader_cache.RegisterShaderProgram( "phong" );
    shader_cache.RegisterShaderProgram( "edge_detect", "simple", "edge_detect" );
    shader_cache.RegisterShaderProgram( "normals", "phong", "normals" );
    shader_cache.RegisterShaderProgram( "tonemap", "fullscreen", "tonemap" );
    shader_cache.RegisterShaderProgram( "texture_full_screen", "fullscreen", "hud" );

    
    printf( "before creating render_target\n");
    render_target = OpenGL::RenderTarget( 1024, 768 );
    printf( "before AttachTexture\n");
    render_target.AttachTexture();
    printf( "after AttachTexture\n");
    OpenGL::GraphicsDevice::SetRenderTarget( render_target );
    OpenGL::GraphicsDevice::Clear(Color::ForestGreen());
    OpenGL::GraphicsDevice::SetDefaultRenderTarget();
    
    printf( "before creating msaa_render_target\n");
    msaa_render_target = OpenGL::RenderTarget( 1024, 768, true );
    printf( "before EnableMultisampling\n");
    msaa_render_target.EnableMultisampling();
    printf( "after EnableMultisampling\n");

    

    
    printf( "before creating float_render_target\n");
    float_render_target = OpenGL::RenderTarget( 1024, 768, true );
    printf( "before AttachTexture\n");
    float_render_target.AttachTexture();
    printf( "after AttachTexture\n");
    OpenGL::GraphicsDevice::SetRenderTarget( render_target );
    OpenGL::GraphicsDevice::Clear(Color::Yellow());
    OpenGL::GraphicsDevice::SetDefaultRenderTarget();
    
   
    
    
    texture_cache.RegisterTexture( "render_target", render_target.GetTexture() );
    texture_cache.RegisterTexture( "float_render_target", float_render_target.GetTexture() );
    
    
    
    
    
    
    
    
    /* BEGIN HUD */
    
    hud_root = new SceneGraph::Node();
    
    auto debug_window = new SceneGraph::Geode();
    debug_window->shader_program = "hud";
    debug_window->vertex_buffer = OpenGL::VertexBuffer<Vertex>( Quad::XYQuadCentered(GeoFloat3(), 256, 256).ToVertices() );
    debug_window->local_transform = GeoMatrix::Translation(1024 - 512 - 128 - 5, 0, 0);
    debug_window->textures["diffuse"] = SceneGraph::TextureDetails("render_target" );
    hud_root->children.push_back(debug_window);
    
    
    
    /* END HUD   */
    
    
    const float court_depth = 4;
    const float pole_height = 1.0f;
    
    
    
    root = new SceneGraph::Node();
    
    
    auto garbage_can = NodeTreeFromWavefrontModel("/Users/mconway/projects/volley/models/can2.obj");
    root->children.push_back(garbage_can);
    
    
    

    auto node = new SceneGraph::Geode();
    node->shader_program = "phong";
    auto quad = Quad::XZQuadCentered(GeoFloat3(), 5, court_depth);
    quad.SquareTesselate();
    quad.SquareTesselate();
    quad.SquareTesselate();
    quad.SquareTesselate();
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>( quad.ToVertices() );
    auto tex_scale = GeoFloat2(6,6);
    node->textures["diffuse"] = SceneGraph::TextureDetails("stones.png", tex_scale);
    node->textures["normal"] = SceneGraph::TextureDetails("stones_normal.png", tex_scale);
    //node->material.specular = GeoFloat3(0,0,0);
    root->children.push_back(node);
    
    
  
    
    
    /* todo:  css/haml like system for defining scene graph... */
    /* todo:  normal mapping */


    auto cylinder_vertex_buffer =  OpenGL::VertexBuffer<Vertex>(  Cylinder(1,1,1).ToVertices() );
    const float large_cylinder_diameter = 0.1f;
    const float small_cylinder_diameter = 0.06f;
    GeoFloat3 cylinder_rim_color;// = GeoFloat3( 1.0f, 0.95f, 0.95f);
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(large_cylinder_diameter, pole_height, large_cylinder_diameter ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, -court_depth / 2.0f );
    node->material.rim = cylinder_rim_color;
    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(small_cylinder_diameter, pole_height/2, small_cylinder_diameter ) *
    GeoMatrix::Translation(0, pole_height, -court_depth / 2.0f );
    node->material.rim = cylinder_rim_color;

    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(large_cylinder_diameter, pole_height, large_cylinder_diameter ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, court_depth / 2.0f );
    node->material.rim = cylinder_rim_color;
    root->children.push_back(node);
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(small_cylinder_diameter, pole_height/2, small_cylinder_diameter ) *
    GeoMatrix::Translation(0, pole_height, court_depth / 2.0f );
    node->material.rim = cylinder_rim_color;
    root->children.push_back(node);
    
    
    auto sphere_vertices = Sphere( ).Transform( GeoMatrix::Scaling(0.25f) ).ToVertices();
    
    auto uv_min_max = FindMinMaxUvs(sphere_vertices);
    
    printf( "min.x, max.x is %f, %f\n", uv_min_max.first.x, uv_min_max.second.x );
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  sphere_vertices );
    node->textures["diffuse"] = SceneGraph::TextureDetails("volleyball.png");
    node->textures["normal"] = SceneGraph::TextureDetails("volleyball_normal.png", GeoFloat2(1, 1) );
    node->local_transform = GeoMatrix::Translation(0, 1, 0 );
    //node->material = SceneGraph::Material::Gold();
    node->material.rim = GeoFloat3( 0.8f, 0.8f, 1.0f );
    node->material.shininess = 64;
    root->children.push_back(node);
    
    auto floor_lines = new SceneGraph::Geode;
    floor_lines->vertex_buffer = OpenGL::VertexBuffer<Vertex>( CalculateDebugTangentLines( sphere_vertices, 0.04f ), OpenGL::PRIMITIVE_TYPE::LINELIST );
    floor_lines->textures["diffuse"] = SceneGraph::TextureDetails("grass.jpg");
    floor_lines->local_transform = GeoMatrix::Translation(0, 1, 0 );
    floor_lines->shader_program = "phong";
    floor_lines->material = SceneGraph::Material::Zero();
    floor_lines->material.emissive = GeoFloat3( 1, 0, 0 );
    
    //root->children.push_back(floor_lines);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  Cube::UnitCube().Transform( GeoMatrix::Scaling(0.3f)).ToVertices() );
    node->textures["diffuse"] = SceneGraph::TextureDetails("stones.png", 2);
    node->textures["normal"] = SceneGraph::TextureDetails("stones_normal.png", 2);
    //node->material.specular = GeoFloat3();
    node->local_transform = GeoMatrix::Translation(-1, 1, 0 );
    root->children.push_back(node);
    
    
    
    node = new SceneGraph::BillboardSprite();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  Quad::XYUnitQuad().ToVertices() );
    node->textures["diffuse"] = SceneGraph::TextureDetails("grass.jpg");
    //node->local_transform = GeoMatrix::Scaling(0.25f) * GeoMatrix::Translation(1, 1, 0 );
    //root->children.push_back(node);
    
    
    // https://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights/
    auto directional_light = new SceneGraph::LightNode;
    directional_light->IDirectionalLight::directional = true;
    directional_light->IDirectionalLight::direction = GeoFloat3( 1, 0, 1 );
    directional_light->IBaseLightDetails::diffuse = GeoFloat3( 0.55f, 0.55f, 0.55f );
    directional_light->IBaseLightDetails::specular = GeoFloat3( 0.53f, 0.53f, 0.54f );
    //root->children.push_back(directional_light);
    
    auto directional_light2 = new SceneGraph::LightNode;
    directional_light2->IDirectionalLight::directional = true;
    directional_light2->IDirectionalLight::direction = GeoFloat3( 0, 0, -1 );
    directional_light2->IBaseLightDetails::diffuse = GeoFloat3( 0.25f, 0.25f, 0.25f );
    directional_light2->IBaseLightDetails::specular = GeoFloat3( 0.13f, 0.13f, 0.14f );
    //root->children.push_back(directional_light2);
    
    
    auto rotor = new SceneGraph::Rotor;
    rotor->local_transform = GeoMatrix::Translation(0, 1, 3);
    rotor->local_rotation_axis = GeoVector( 0, 1, 0 );
    rotor->local_rotation_speed = 0;
    root->children.push_back(rotor);
    
    
    // https://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights/
    auto light = new SceneGraph::LightGeode();
    light->shader_program = "phong";
    light->textures["diffuse"] = SceneGraph::TextureDetails("grass.jpg");
    light->IBaseLightDetails::diffuse = GeoFloat3( 0.55f, 0.55f, 0.55f );
    light->IBaseLightDetails::specular = GeoFloat3( 0.63f, 0.63f, 0.64f );
    light->material.emissive = GeoFloat3( 1, 1, 1 );
    light->vertex_buffer = OpenGL::VertexBuffer<Vertex>( Cube::UnitCube().Transform( GeoMatrix::Scaling(0.1f)).ToVertices() );
    rotor->children.push_back(light);
    
    
    
    
}

Camera* Scene::GetCamera(){
    return camera;
}




void Scene::ConfigureShaderProgram( SceneGraph::Node* node, SceneGraph::IDrawable* drawable, Camera* cam, const std::string& override_shader_program  ){
    auto shader_program = override_shader_program.size() ? override_shader_program : drawable->shader_program;
    
    
    drawable->vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( shader_program, sizeof(Vertex) );
    
    
    shader_cache.SetInt( "fudge", fudge);
    shader_cache.SetInt( "fudge2", fudge2);

    
    // set material
    shader_cache.SetFloat3( "material.ambient", drawable->material.ambient );
    shader_cache.SetFloat3( "material.diffuse", drawable->material.diffuse );
    shader_cache.SetFloat3( "material.specular", drawable->material.specular );
    shader_cache.SetFloat3( "material.emissive", drawable->material.emissive );
    shader_cache.SetFloat3( "material.rim", drawable->material.rim );
    shader_cache.SetFloat( "material.shininess", drawable->material.shininess );
    
    
    
    std::vector<SceneGraph::TextureDetails> texture_details;
    texture_details.push_back( drawable->textures["diffuse"] );
    texture_details.push_back( drawable->textures["normal"] );
    texture_details.push_back( global_textures["ramp"] );
    
    for( int i = 0; i < texture_details.size(); i++ ){
        auto tex_details = texture_details[i];
        if( tex_details.texture_name.size() ){
            auto sampler_name = StringSubstituteNumber( "tex%i", i+1);
            auto tex = texture_cache.FromName(tex_details.texture_name);
            shader_cache.SetTexture( sampler_name, tex, i);
            shader_cache.SetFloat2( sampler_name + "_scale", tex_details.scale );
            shader_cache.SetFloat2( sampler_name + "_dimensions", tex.GetWidth(), tex.GetHeight() );
        }
    }
   
    //todo: research uniform buffer objects to set all uniforms in one call
    auto transform = node->cached_world_transform;
    shader_cache.SetMatrix( "view_transform", cam->GetViewTransform() );
    shader_cache.SetMatrix( "projection_transform", cam->GetProjectionTransform() );
    shader_cache.SetMatrix( "world_transform", transform );
    shader_cache.SetMatrix( "world_inverse_transpose", transform.Inverse(0).Transpose() );
    
    shader_cache.SetFloat( "viewport_width", cam->GetWidth() );
    shader_cache.SetFloat( "viewport_height", cam->GetHeight() );
    shader_cache.SetFloat3( "eye_position", cam->GetEyePosition() );
    
    shader_cache.SetInt("num_lights", (int) light_nodes.size() );
    
   // printf( "num lights is %i\n", (int) light_nodes.size() );
    
    for( int i = 0; i < light_nodes.size(); i++ ){
        auto light = light_nodes[i];
        auto prefix = StringSubstituteNumber( "lights[%i].", i);
        
        shader_cache.SetFloat3( prefix + "ambient", light->IBaseLightDetails::ambient );
        shader_cache.SetFloat3( prefix + "diffuse", light->IBaseLightDetails::diffuse );
        shader_cache.SetFloat3( prefix + "specular", light->IBaseLightDetails::specular );
        if( light->IDirectionalLight::directional ){
            shader_cache.SetFloat4( prefix + "position", GeoVector(light->IDirectionalLight::direction, 0) );
        }
        else{
            shader_cache.SetFloat4( prefix + "position", GeoVector(light->cached_world_transform.GetTranslationComponent(), 1));
            shader_cache.SetFloat3( prefix + "attenuation", light->IPositionalLight::attenuation.ToGeoFloat3() );
            if( light->spotlight ){
                shader_cache.SetFloat( prefix + "cone_angle", light->ISpotlightDetails::cone_angle );
            }
        }
    }
}


void Scene::Update( const float elapsed_seconds ){
    light_nodes.clear();
    UpdateNodes( root, GeoMatrix::Identity(), elapsed_seconds );
    UpdateNodes( hud_root, GeoMatrix::Identity(), elapsed_seconds );
}

void Scene::Draw(){
    // render scene into float render target
    OpenGL::GraphicsDevice::GetStateManager().EnableDepthTest();
    DrawNodesToRenderTarget( root, GetCamera(), msaa_render_target );
    
    msaa_render_target.BlitToRenderTarget( float_render_target );
    
    // tonemap float render target and render into int render target
    OpenGL::GraphicsDevice::GetStateManager().DisableDepthTest();
    DrawFullScreenTextureToRenderTarget( float_render_target.GetTexture(), render_target, "tonemap");
    
  
    OpenGL::GraphicsDevice::Clear( Color::Beige() );
    OpenGL::GraphicsDevice::GetStateManager().EnableDepthTest();
    //DrawNodesToScreen( root, GetCamera() );
    
    OpenGL::GraphicsDevice::GetStateManager().DisableDepthTest();
    render_target.BlitToScreen(screen_width, screen_height);
    //DrawFullScreenTextureToScreen( render_target.GetTexture(), "texture_full_screen" );
    
    OpenGL::GraphicsDevice::GetStateManager().DisableDepthTest();
    DrawNodesToScreen( hud_root, hud_camera, "hud" );
}

void Scene::UpdateNodes( SceneGraph::Node* node, GeoMatrix transform, const float elapsed_seconds ){
    node->Update( elapsed_seconds );
    
    
    auto new_transform = transform * node->GetUpdatedLocalTransform();
    auto configured_transform = new_transform;
    
    if( dynamic_cast<SceneGraph::LightNode*>(node) || dynamic_cast<SceneGraph::LightGeode*>(node) )
        light_nodes.push_back( static_cast<SceneGraph::LightNode*>(node) );
    
    
    auto geode = dynamic_cast<SceneGraph::Geode*>(node);
    if( geode ){
        auto billboard = dynamic_cast<SceneGraph::BillboardSprite*>( geode );
        if( billboard )
            configured_transform = camera->GetConstrainedBillboardTransform( transform.GetTranslationComponent() );
    }
    
    if( !node->cached_world_transform_set )
        node->cached_world_transform = configured_transform;
    
    for( int i = 0; i < node->children.size(); i++ ){
        UpdateNodes( node->children[i], new_transform, elapsed_seconds );
    }
}

void Scene::DrawFullScreenTextureToRenderTarget( OpenGL::Texture texture, OpenGL::RenderTarget render_target, const std::string& shader_program ){
    auto geode_and_camera = BuildFullScreenNode( texture );
    DrawNodesToRenderTarget( &geode_and_camera.first, &geode_and_camera.second, render_target, shader_program );
}

void Scene::DrawFullScreenTextureToScreen( OpenGL::Texture texture, const std::string& shader_program ){
    auto geode_and_camera = BuildFullScreenNode( texture );
    DrawNodesToScreen( &geode_and_camera.first, &geode_and_camera.second, shader_program );
}

std::pair< SceneGraph::Geode, HudCamera > Scene::BuildFullScreenNode( OpenGL::Texture texture){
    texture_cache.RegisterTexture("fullscreen", texture);
    
    SceneGraph::Geode geode;
    geode.vertex_buffer = unit_quad_vertex_buffer;
    geode.textures["diffuse"] = SceneGraph::TextureDetails( "fullscreen" );
    
    return std::tuple< SceneGraph::Geode, HudCamera >( geode, HudCamera(1,1));
}



void Scene::DrawNodesToRenderTarget( SceneGraph::Node* node, Camera* cam, OpenGL::RenderTarget render_target ){
    DrawNodesToRenderTarget(node, cam, render_target, "");
}

void Scene::DrawNodesToRenderTarget( SceneGraph::Node* node, Camera* cam, OpenGL::RenderTarget rt, const std::string& override_shader_program ){
    OpenGL::GraphicsDevice::SetRenderTarget( rt );
    OpenGL::GraphicsDevice::SetViewport( rt.GetWidth(), rt.GetHeight() );
    OpenGL::GraphicsDevice::Clear(Color::ForestGreen());
    
    //cam->SetWidthHeight( render_target.GetWidth(), render_target.GetHeight() );
    DrawNodes( node, cam, override_shader_program );
    OpenGL::GraphicsDevice::SetDefaultRenderTarget();
    
}

void Scene::DrawNodesToScreen( SceneGraph::Node* node, Camera* cam ){
    DrawNodesToScreen(node, cam, "");
}

void Scene::DrawNodesToScreen( SceneGraph::Node* node, Camera* cam, const std::string& override_shader_program  ){
    OpenGL::GraphicsDevice::SetViewport( screen_width, screen_height );
    cam->SetWidthHeight( screen_width, screen_height );
    DrawNodes( node, cam, override_shader_program );
}



void Scene::DrawNodes( SceneGraph::Node* node, Camera* cam, const std::string& override_shader_program ){
    auto drawable = dynamic_cast<SceneGraph::IDrawable*>(node);
    
    if( drawable ){
        ConfigureShaderProgram( node, drawable, cam, override_shader_program );
        drawable->vertex_buffer.Draw();
    }
    
    for( int i = 0; i < node->children.size(); i++ ){
        DrawNodes( node->children[i], cam, override_shader_program );
    }
}


