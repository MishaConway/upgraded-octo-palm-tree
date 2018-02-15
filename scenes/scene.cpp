#include "scene.h"
#include "../ogl/device/device.h"
#include "../string/string_utils.h"
#include "../shapes/shape_utilities/debug_lines.h"

//https://www.keithlantz.net/2011/10/tangent-space-normal-mapping-with-glsl/

void Scene::Initialize( const unsigned int width, const unsigned int height ){
    fudge = 0;
    
    
    camera = new FirstPersonCamera();
    camera->SetProjection( width, height, 45.0f, 0.1f, 100.0f );
    camera->SetEyePosition(GeoVector(-1, 0.5f, 4 ));
    
    //camera->SetFocusPosition(GeoVector( 0, 1, 0 ));

    
    //shader_cache.RegisterShaderProgram( "basic" );
    shader_cache.RegisterShaderProgram( "phong" );
    
    render_target = OpenGL::RenderTarget( 512, 512 );
    
    
    
    const float court_depth = 4;
    const float pole_height = 1.0f;
    
    
    root = new SceneGraph::Node();
    
    
    

    auto node = new SceneGraph::Geode();
    node->shader_program = "phong";
    auto quad = Quad::XZQuadCentered(GeoFloat3(), 5, court_depth);
    //quad.SquareTesselate();
    //quad.SquareTesselate();
    //quad.SquareTesselate();
    //quad.SquareTesselate();
    printf( "START VERTS FOR FLOOR..\n");
    node->vertex_buffer = OpenGL::VertexBuffer<Vertex>( quad.ToVertices() );
    printf( "END VERTS FOR FLOOR..\n");

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
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(large_cylinder_diameter, pole_height, large_cylinder_diameter ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, -court_depth / 2.0f );
    root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(small_cylinder_diameter, pole_height/2, small_cylinder_diameter ) *
    GeoMatrix::Translation(0, pole_height, -court_depth / 2.0f );
    //root->children.push_back(node);
    
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(large_cylinder_diameter, pole_height, large_cylinder_diameter ) *
                            GeoMatrix::Translation(0, pole_height / 2.0f, court_depth / 2.0f );
    root->children.push_back(node);
    
    node = new SceneGraph::Geode();
    node->shader_program = "phong";
    node->vertex_buffer = cylinder_vertex_buffer;
    node->textures["diffuse"] = SceneGraph::TextureDetails("metal1.jpg");
    node->local_transform = GeoMatrix::Scaling(small_cylinder_diameter, pole_height/2, small_cylinder_diameter ) *
    GeoMatrix::Translation(0, pole_height, court_depth / 2.0f );
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
    //node->material = SceneGraph::Material::Silver();
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
    node->textures["diffuse"] = SceneGraph::TextureDetails("stones.png", tex_scale);
    node->textures["normal"] = SceneGraph::TextureDetails("stones_normal.png", tex_scale);
    node->local_transform = GeoMatrix::Translation(-1, 1, 0 );
    root->children.push_back(node);
    
    
    
    node = new SceneGraph::BillboardSprite();
    node->shader_program = "phong";
    node->textures["diffuse"] = SceneGraph::TextureDetails("grass.jpg");
    node->local_transform = GeoMatrix::Scaling(0.25f) * GeoMatrix::Translation(1, 1, 0 );
    root->children.push_back(node);
    
    
    // https://www.tomdalling.com/blog/modern-opengl/08-even-more-lighting-directional-lights-spotlights-multiple-lights/
    auto directional_light = new SceneGraph::LightNode;
    directional_light->IDirectionalLight::directional = true;
    directional_light->IDirectionalLight::direction = GeoFloat3( 1, 0, 1 );
    directional_light->IBaseLightDetails::diffuse = GeoFloat3( 0.55f, 0.55f, 0.55f );
    directional_light->IBaseLightDetails::specular = GeoFloat3( 0.53f, 0.53f, 0.54f );
    root->children.push_back(directional_light);
    
    auto directional_light2 = new SceneGraph::LightNode;
    directional_light2->IDirectionalLight::directional = true;
    directional_light2->IDirectionalLight::direction = GeoFloat3( 0, 0, -1 );
    directional_light2->IBaseLightDetails::diffuse = GeoFloat3( 0.25f, 0.25f, 0.25f );
    directional_light2->IBaseLightDetails::specular = GeoFloat3( 0.13f, 0.13f, 0.14f );
    root->children.push_back(directional_light2);
    
    
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




void Scene::ConfigureShaderProgram( SceneGraph::Node* node, SceneGraph::IDrawable* drawable  ){
    auto shader_program = drawable->shader_program;
    
    drawable->vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( shader_program, sizeof(Vertex) );
    
    
    shader_cache.SetInt( "fudge", fudge);
    
    // set material
    shader_cache.SetFloat3( "material.ambient", drawable->material.ambient );
    shader_cache.SetFloat3( "material.diffuse", drawable->material.diffuse );
    shader_cache.SetFloat3( "material.specular", drawable->material.specular );
    shader_cache.SetFloat3( "material.emissive", drawable->material.emissive );
    shader_cache.SetFloat( "material.shininess", drawable->material.shininess );
    
    
    
   
    
    auto diffuse_tex_details = drawable->textures["diffuse"];
    if( diffuse_tex_details.texture_name.size() ){
        auto diffuse_tex = texture_cache.FromFile(diffuse_tex_details.texture_name);
        shader_cache.SetTexture("tex1", diffuse_tex, 0);
        shader_cache.SetFloat2("tex1_scale", diffuse_tex_details.scale );
    }
    
    auto normal_tex_details = drawable->textures["normal"];
    if( normal_tex_details.texture_name.size() ){
        auto normal_tex = texture_cache.FromFile(normal_tex_details.texture_name);
        shader_cache.SetTexture("tex2", normal_tex, 1);
        shader_cache.SetFloat2("tex2_scale", normal_tex_details.scale );
    }
    
    
    //todo: research uniform buffer objects to set all uniforms in one call
    auto transform = node->cached_world_transform;
    shader_cache.SetMatrix( "view_transform", camera->GetViewTransform() );
    shader_cache.SetMatrix( "projection_transform", camera->GetProjectionTransform() );
    shader_cache.SetMatrix( "world_transform", transform );
    shader_cache.SetMatrix( "world_inverse_transpose", transform.Inverse(0).Transpose() );
    
    shader_cache.SetFloat( "viewport_width", camera->GetWidth() );
    shader_cache.SetFloat( "viewport_height", camera->GetHeight() );
    shader_cache.SetFloat3( "eye_position", camera->GetEyePosition() );
    
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
    
    if( !node->cached_world_transform_set )
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


