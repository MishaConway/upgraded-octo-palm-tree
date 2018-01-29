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



void Scene::Initialize( const unsigned int width, const unsigned int height ){
    camera = Camera( width, height, 45.0f, 0.1f, 100.0f, GeoVector(0, 0, 2 ), GeoVector( 0, 0, 0 ) );

    
    shader_cache.RegisterShaderProgram( "basic" );
    
    
    
    root = new SceneGraph::Node();
    
    SceneGraph::Geode* node = new SceneGraph::Geode();
    node->shader_program = "basic";
    node->vertex_buffer =OpenGL::VertexBuffer<Vertex>( Quad::XYUnitQuad().ToVertices() );
    node->textures["diffuse"] = "grass.jpg";
    root->children.push_back(node);
}

void Scene::Update( unsigned int elapsed_milliseconds ){
    
    
}


void Scene::ConfigureShaderProgram( SceneGraph::Geode* geode ){
    geode->vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( "basic", sizeof(Vertex) );
    
    auto texture = texture_cache.FromFile("grass.jpg");
    shader_cache.SetTexture("lala", texture, 0);
    
    
    shader_cache.SetMatrix( "ViewTransform", camera.GetViewTransform() );
    shader_cache.SetMatrix( "ProjectionTransform", camera.GetProjectionTransform() );
    shader_cache.SetMatrix( "WorldTransform", GeoMatrix::Identity() );
    
    shader_cache.SetFloat( "viewport_width", 640 );
    shader_cache.SetFloat( "viewport_height", 480 );
    shader_cache.SetFloat3( "eye_position", camera.GetEyePosition() );
}




void Scene::Draw(){
    OpenGL::GraphicsDevice::Clear( Color::DeepPink() );
   
    TraverseNodes(GeoMatrix::Identity(), root);
}

void Scene::TraverseNodes( GeoMatrix transform, SceneGraph::Node* node ){
    auto geode = dynamic_cast<SceneGraph::Geode*>(node);
    if( geode ){
        ConfigureShaderProgram( geode );
        geode->vertex_buffer.Draw();
    }
    
    
    for( int i = 0; i < node->children.size(); i++ ){
        TraverseNodes( transform, node->children[i] );
    }
}


