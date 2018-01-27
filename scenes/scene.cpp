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
    
    camera = Camera( 640, 480, 45.0f, 0.1f, 100.0f, GeoVector(0, 7, 14 ), GeoVector( 0, 0, 0 ) );
    
    vertex_buffer = OpenGL::VertexBuffer<Vertex>( RoundedCube::UnitRoundedCube(14, 0.15f).ToVertices() );
}



void Scene::Draw(){
    
    vertex_buffer.Bind();
    shader_cache.ActivateShaderProgram( "basic", sizeof(Vertex) );
    
    
    shader_cache.SetMatrix( "ViewTransform", camera.GetViewTransform() );
    shader_cache.SetMatrix( "ProjectionTransform", camera.GetProjectionTransform() );
    shader_cache.SetMatrix( "WorldTransform", GeoMatrix::Identity() );

    shader_cache.SetFloat( "viewport_width", 640 );
    shader_cache.SetFloat( "viewport_height", 480 );
    shader_cache.SetFloat3( "eye_position", camera.GetEyePosition() );
    
    
    
    
    glLoadIdentity();
    glTranslatef(-2, 0.0f, -6.0f);
    
    /*
     glBegin(GL_TRIANGLES);
     glVertex3f(0.0f, 1.0f, 0.0f);
     glVertex3f(-1.0f, -1.0f, 0.0f);
     glVertex3f(1.0f, -1.0f, 0.0f);
     glEnd();*/
    
    
    /*
     glTranslatef(3.0f, 0.0f, 0.0f);
     
     glBegin(GL_QUADS);
     glVertex3f(-1.0f, 1.0f, 0.0f);
     glVertex3f(1.0f, 1.0f, 0.0f);
     glVertex3f(1.0f, -1.0f, 0.0f);
     glVertex3f(-1.0f, -1.0f, 0.0f);
     glEnd(); */
    
    Triangle tri( GeoFloat3(0, 1, 0),
                 GeoFloat3(-1, -1, 0),
                 GeoFloat3(1, -1, 0) );
    
    Quad quad = Quad::XYUnitQuad();
    
    Cube cube = Cube::UnitCube();
    
    RoundedCube rounded_cube = RoundedCube::UnitRoundedCube(14, 0.15f);
    
    Sphere sphere;
    
    //glFrontFace(GL_CCW);
    
    glDisable(GL_CULL_FACE);
    
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    //DebugDraw(tri.ToVertices());
    //DebugDraw(rounded_cube.ToVertices());
    
    
    vertex_buffer.Draw();
    
    
    
}


