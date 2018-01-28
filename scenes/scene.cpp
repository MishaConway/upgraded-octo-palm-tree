#include "scene.h"



void DebugDraw( std::vector<Vertex> vertices ){
    glBegin(GL_TRIANGLES);
    for( int i = 0; i < vertices.size(); i+= 3 ){
        glVertex3f( vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
        glVertex3f( vertices[i+1].position.x, vertices[i+1].position.y, vertices[i+1].position.z);
        glVertex3f( vertices[i+2].position.x, vertices[i+2].position.y, vertices[i+2].position.z);
    }
    glEnd();
}



void Scene::Initialize(){
    glewInit(); //todo: remove this once using device...
    
    
    shader_cache.RegisterShaderProgram( "basic" );
    
    texture = OpenGL::Texture( "/Users/mconway/projects/volley/images/grass.jpg" );

    
    camera = Camera( 640, 480, 45.0f, 0.1f, 100.0f, GeoVector(0, 0, 2 ), GeoVector( 0, 0, 0 ) );
    
    Triangle tri( GeoFloat3(0, 1, 0),
                 GeoFloat3(-1, -1, 0),
                 GeoFloat3(1, -1, 0) );
    
    Quad quad = Quad::XYUnitQuad();
    
    Cube cube = Cube::UnitCube();
    
    RoundedCube rounded_cube = RoundedCube::UnitRoundedCube(14, 0.15f);
    
    Sphere sphere;
    
    vertex_buffer = OpenGL::VertexBuffer<Vertex>( quad.ToVertices() );
}

void Scene::ConfigureShaderProgram( OpenGL::VertexBuffer<Vertex>& vertex_buffer ){
    vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( "basic", sizeof(Vertex) );
    
    
    shader_cache.SetMatrix( "ViewTransform", camera.GetViewTransform() );
    shader_cache.SetMatrix( "ProjectionTransform", camera.GetProjectionTransform() );
    shader_cache.SetMatrix( "WorldTransform", GeoMatrix::Identity() );
    
    shader_cache.SetFloat( "viewport_width", 640 );
    shader_cache.SetFloat( "viewport_height", 480 );
    shader_cache.SetFloat3( "eye_position", camera.GetEyePosition() );
    
    shader_cache.SetTexture("lala", texture, 0);
}




void Scene::Draw(){
    ConfigureShaderProgram(vertex_buffer);
    vertex_buffer.Draw();
}


