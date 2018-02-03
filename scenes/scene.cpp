#include "scene.h"
#include "../ogl/device/device.h"



void DebugDraw( std::vector<Vertex> vertices ){
    glBegin(GL_TRIANGLES);
    for( int i = 0; i < vertices.size(); i+= 3 ){
        glVertex3f( vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
        glVertex3f( vertices[i+1].position.x, vertices[i+1].position.y, vertices[i+1].position.z);
        glVertex3f( vertices[i+2].position.x, vertices[i+2].position.y, vertices[i+2].position.z);
    }
    glEnd();
}

Camera* Scene::GetCamera(){
    return camera;
}



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
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    
    
    //glEnable(GL_POLYGON_SMOOTH);
}

void Scene::Update( const float elapsed_seconds ){
    //camera.Update( elapsed_seconds );
}


void Scene::ConfigureShaderProgram( SceneGraph::Geode* geode, GeoMatrix transform ){
    geode->vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( geode->shader_program, sizeof(Vertex) );
    
    auto texture = texture_cache.FromFile("volleyball.png");
    
    static bool saved = false;
    if( !saved )
        texture.SaveToFile("blah.jpg");
    saved = true;
    
    shader_cache.SetTexture("tex1", texture, 0);
    shader_cache.SetFloat2("tex1_scale", GeoFloat2(1,1) );
    
    
    
    shader_cache.SetMatrix( "view_transform", camera->GetViewTransform() );
    shader_cache.SetMatrix( "projection_transform", camera->GetProjectionTransform() );
    shader_cache.SetMatrix( "world_transform", transform );
    shader_cache.SetMatrix( "world_inverse_transpose", transform.Inverse(0).Transpose() );
    
    shader_cache.SetFloat( "viewport_width", camera->GetWidth() );
    shader_cache.SetFloat( "viewport_height", camera->GetHeight() );
    shader_cache.SetFloat3( "eye_position", camera->GetEyePosition() );
}




void Scene::Draw(){
    OpenGL::GraphicsDevice::Clear( Color::Beige() );
   
    TraverseNodes( root, GeoMatrix::Identity() );
}

void Scene::TraverseNodes( SceneGraph::Node* node, GeoMatrix transform ){
    glDisable(GL_CULL_FACE);

    
    auto new_transform = transform * node->local_transform;
    
    auto geode = dynamic_cast<SceneGraph::Geode*>(node);
    if( geode ){
        auto configured_transform = new_transform;
        
        auto billboard = dynamic_cast<SceneGraph::BillboardSprite*>( geode );
        if( billboard )
            configured_transform = camera->GetConstrainedBillboardTransform( transform.GetTranslationComponent() );
        
        ConfigureShaderProgram( geode, configured_transform );
        geode->vertex_buffer.Draw();
    }
    
    
    for( int i = 0; i < node->children.size(); i++ ){
        TraverseNodes( node->children[i], new_transform );
    }
}


