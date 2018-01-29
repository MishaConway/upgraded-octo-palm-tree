/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <iostream>
#include "glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/GLU.h>


#include "../ogl/device/device.h"

#include "../io/file_io.h"

#include "../shapes/shape.h"
#include "../shapes/triangle.h"
#include "../shapes/quad.h"
#include "../shapes/cube.h"
#include "../shapes/rounded_cube.h"
#include "../shapes/sphere.h"


#include "../scenes/scene.h"
#include "../scenes/shader_cache/shader_cache.h"
#include "../scenes/texture_cache/texture_cache.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480




int main(int argc, char *argv[])
{
    TextureCache::ConfigureTexturePaths("/Users/mconway/projects/volley/images");
    ShaderCache::ConfigureShaderPaths("/Users/mconway/projects/volley/shaders/vertex", "/Users/mconway/projects/volley/shaders/pixel");
    
    SDL_Window* gWindow = NULL;
    SDL_GLContext gGlContext;
    
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL cannot init with error " << SDL_GetError() << std::endl;
        return -1;
    }
    
    // set opengl version to use in this program
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
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
    
    OpenGL::GraphicsDevice::Initialize();
    
 
    Scene scene;
    scene.Initialize( SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Uint32 time = 0;
    
    
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
        
        auto old_time = time;
        time = SDL_GetTicks();
        
        Uint32 elapsed_time = 0;
        if( old_time > 0 ){
            elapsed_time = time - old_time;
        }
        
        scene.Update( elapsed_time );
        
        // render stuff here
        scene.Draw();
        
        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }
    
    // clear resource
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    SDL_Quit();
    
    return 0;
}