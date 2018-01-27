/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <iostream>
#include "glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/GLU.h>

#include "../io/file_io.h"

#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"
#include "../shapes/sphere.h"


#include "../scenes/scene.h"
#include "../scenes/shader_cache/shader_cache.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = NULL;
SDL_GLContext gGlContext;


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



void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    ShaderCache::ConfigureShaderPaths("/Users/mconway/projects/volley/shaders/vertex", "/Users/mconway/projects/volley/shaders/pixel");
    
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
    
    
    Scene scene;
    scene.Initialize();
    
    
    
    
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
        scene.Draw();
        
        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }
    
    // clear resource
    close();
    
    return 0;
}