/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/GLU.h>
#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = NULL;
SDL_GLContext gGlContext;


void DebugDraw( std::vector<Vertex> vertices ){
    glBegin(GL_TRIANGLES);
    for( int i = 0; i < vertices.size(); i+= 3 ){
        glVertex3f( vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
        glVertex3f( vertices[i+1].position.x, vertices[i+1].position.y, vertices[i+1].position.z);
        glVertex3f( vertices[i+2].position.x, vertices[i+2].position.y, vertices[i+2].position.z);
    }
    glEnd();
}

void SetOpenGLVersion()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
}

void Display_InitGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int Display_SetViewport(int width, int height)
{
    GLfloat ratio;
    
    if (height == 0) {
        height = 1;
    }
    
    ratio = GLfloat(width) / GLfloat(height);
    glViewport(0, 0, GLsizei(width), GLsizei(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    return true;
}

void Display_Render()
{
    glLoadIdentity();
    glTranslatef(2, 0.0f, -6.0f);

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
    
    RoundedCube rounded_cube = RoundedCube::UnitRoundedCube(40, 0.3f);
    
    //glFrontFace(GL_CCW);
    
    glDisable(GL_CULL_FACE);
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    //DebugDraw(tri.ToVertices());
    DebugDraw(rounded_cube.ToVertices());
    

    
    
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL cannot init with error " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // set opengl version to use in this program
    SetOpenGLVersion();
    
    // create window
    gWindow = SDL_CreateWindow("Opengl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        std::cout << "Cannot create window with error " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // create opengl context
    gGlContext = SDL_GL_CreateContext(gWindow);
    if (gGlContext == NULL)
    {
        std::cout << "Cannot create OpenGL context with error " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // initialize opengl
    Display_InitGL();
    // set camera
    Display_SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    
    Triangle tri( GeoFloat3(0, 1, 0),
                 GeoFloat3(-1, -1, 0),
                 GeoFloat3(1, -1, 0) );
    

    
    bool quit = false;
    SDL_Event sdlEvent;
    while (!quit)
    {
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        
        // clear bg color and depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render stuff here
        Display_Render();
        
        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }
    
    // clear resource
    close();
    
    return 0;
}