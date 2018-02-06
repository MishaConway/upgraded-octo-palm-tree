#include "scene.h"
#include "../ogl/device/device.h"



void Scene::Initialize( const unsigned int width, const unsigned int height ){
    camera = new FirstPersonCamera();
    camera->SetProjection( width, height, 45.0f, 0.1f, 100.0f );
    camera->SetEyePosition(GeoVector(-1, 0.5f, 4 ));
    
    //camera->SetFocusPosition(GeoVector( 0, 1, 0 ));

    
    shader_cache.RegisterShaderProgram( "basic" );
    shader_cache.RegisterShaderProgram( "phong" );
    
    render_target = OpenGL::RenderTarget( 512, 512 );
    
    
    
    const float court_depth = 4;
    const float pole_height = 1.0f;
    
    
    root = new SceneGraph::Node();
    
    SceneGraph::Geode* node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>( Quad::XYUnitQuad().ToVertices() );
    node->textures["diffuse"] = "grass.jpg";
    //root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    auto quad = Quad::XZQuadCentered(GeoFloat3(), 5, court_depth);
    quad.SquareTesselate();
    quad.SquareTesselate();
    quad.SquareTesselate();
    quad.SquareTesselate();
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>( quad.ToVertices() );
    node->textures["diffuse"] = "grass.jpg";
    root->children.push_back(node);
    
    auto verts = Quad::XZQuadCentered(GeoFloat3(), 5, court_depth).ToVertices();
    for( int i = 0; i < verts.size(); i++ ){
        printf( "vert normal is %f, %f, %f\n", verts[i].normal.x, verts[i].normal.y, verts[i].normal.z );
    }
    
    /* todo:  css/haml like system for defining scene graph... */
    /* todo:  normal mapping */
    /* todo:  bill boarding */



    
   /* node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  Cylinder( 0.2f, pole_height, 0.2f ).ToVertices() );
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Translation(0, pole_height / 2.0f, -court_depth / 2.0f );
    root->children.push_back(node); */
    
    auto cylinder_vertex_buffer =  OpenGL::VertexBuffer<Vertex>(  Cylinder(1,1,1).ToVertices() );
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Scaling(0.2f, pole_height, 0.2f ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, -court_depth / 2.0f );
    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Scaling(0.1f, pole_height/2, 0.1f ) *
    GeoMatrix::Translation(0, pole_height, -court_depth / 2.0f );
    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Scaling(0.2f, pole_height, 0.2f ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, court_depth / 2.0f );
    root->children.push_back(node);
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Scaling(0.1f, pole_height/2, 0.1f ) *
    GeoMatrix::Translation(0, pole_height, court_depth / 2.0f );
    root->children.push_back(node);
    
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  Sphere( ).Transform( GeoMatrix::Scaling(0.45f) ).ToVertices() );
    node->textures["diffuse"] = "volleyball.png";
    node->local_transform = GeoMatrix::Translation(0, 1, 0 );
    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>(  RoundedCube::UnitRoundedCube().Transform( GeoMatrix::Scaling(0.3f)).ToVertices() );
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Translation(-1, 1, 0 );
    root->children.push_back(node);
    
    
    
    node = new SceneGraph::BillboardSprite();
    node->shader_program = "phong";
    node->textures["diffuse"] = "grass.jpg";
    node->local_transform = GeoMatrix::Scaling(0.25f) * GeoMatrix::Translation(1, 1, 0 );
    root->children.push_back(node);
    
    
    auto rotor = new SceneGraph::Rotor;
    rotor->local_transform = GeoMatrix::Translation(1, 1, 0);
    rotor->local_rotation_axis = GeoVector( 0, 1, 0 );
    rotor->local_rotation_speed = 10;
    root->children.push_back(rotor);
    
    
    
    
    
    // https://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights/
    auto light = new SceneGraph::LightGeode();
    //light->position = GeoVector( 0, 0.01f, 1 );
    light->directional = false;
    //light->local_transform = GeoMatrix::Translation( 2, 1, 0 );
    light->shader_program = "phong";
    light->textures["diffuse"] = "grass.jpg";
    light->vertex_buffer = OpenGL::VertexBuffer<Vertex>( Cube::UnitCube().Transform( GeoMatrix::Scaling(0.1f)).ToVertices() );
    rotor->children.push_back(light);
    
    
    
    
}

Camera* Scene::GetCamera(){
    return camera;
}




void Scene::ConfigureShaderProgram( SceneGraph::Node* node, SceneGraph::IDrawable* drawable  ){
    auto transform = node->cached_world_transform;
    
    drawable->vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( drawable->shader_program, sizeof(Vertex) );
    
    // set material
    shader_cache.SetFloat3( "material.ambient", drawable->material.ambient );
    shader_cache.SetFloat3( "material.diffuse", drawable->material.diffuse );
    shader_cache.SetFloat3( "material.specular", drawable->material.specular );
    shader_cache.SetFloat( "material.shininess", drawable->material.shininess );
    
    
    auto texture = texture_cache.FromFile("volleyball.png");
    
    static bool saved = false;
    if( !saved )
        texture.SaveToFile("blah.jpg");
    saved = true;
    
    auto tex = texture_cache.FromFile(drawable->textures["diffuse"]);
    shader_cache.SetTexture("tex1", tex, 0);
    shader_cache.SetFloat2("tex1_scale", GeoFloat2(1,1) );
    
    //todo: research uniform buffer objects to set all uniforms in one call
    
    shader_cache.SetMatrix( "view_transform", camera->GetViewTransform() );
    shader_cache.SetMatrix( "projection_transform", camera->GetProjectionTransform() );
    shader_cache.SetMatrix( "world_transform", transform );
    shader_cache.SetMatrix( "world_inverse_transpose", transform.Inverse(0).Transpose() );
    
    shader_cache.SetFloat( "viewport_width", camera->GetWidth() );
    shader_cache.SetFloat( "viewport_height", camera->GetHeight() );
    shader_cache.SetFloat3( "eye_position", camera->GetEyePosition() );
    
    for( int i = 0; i < light_nodes.size(); i++ ){
        auto light = light_nodes[i];
        shader_cache.SetFloat3( "lights[0].ambient", light->IBaseLightDetails::ambient );
        shader_cache.SetFloat3( "lights[0].diffuse", light->IBaseLightDetails::diffuse );
        shader_cache.SetFloat3( "lights[0].specular", light->IBaseLightDetails::specular );
        if( light->IDirectionalLight::directional ){
            shader_cache.SetFloat4( "lights[0].position", GeoVector(light->IDirectionalLight::direction, 0) );
        }
        else{
            shader_cache.SetFloat4( "lights[0].position", GeoVector(light->cached_world_transform.GetTranslationComponent(), 1));
            if( light->spotlight ){
                shader_cache.SetFloat( "lights[0].cone_angle", light->ISpotlightDetails::cone_angle );
            }
        }
    }
}


void Scene::Update( const float elapsed_seconds ){
    light_nodes.clear();
    UpdateNodes( root, GeoMatrix::Identity(), elapsed_seconds );
}

void Scene::Draw(){
    OpenGL::GraphicsDevice::Clear( Color::Beige() );
    DrawNodes( root );
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
    
    node->cached_world_transform = configured_transform;
    
    for( int i = 0; i < node->children.size(); i++ ){
        UpdateNodes( node->children[i], new_transform, elapsed_seconds );
    }
}


void Scene::DrawNodes( SceneGraph::Node* node ){
    auto drawable = dynamic_cast<SceneGraph::IDrawable*>(node);
    
    if( drawable ){
        ConfigureShaderProgram( node, drawable );
        drawable->vertex_buffer.Draw();
    }
    
    for( int i = 0; i < node->children.size(); i++ ){
        DrawNodes( node->children[i] );
    }
}


