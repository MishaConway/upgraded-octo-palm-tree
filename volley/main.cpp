/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <iostream>
#include "glew.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_gamecontroller.h>
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
#include "../scenes/camera/camera.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768



struct Direction{
    bool left;
    bool right;
    bool up;
    bool down;
    bool forward;
    bool backward;
    bool turning_left;
    bool turning_right;
    bool looking_up;
    bool looking_down;
    
    Direction(){
        left = false;
        right = false;
        up = false;
        down = false;
        forward = false;
        backward = false;
        turning_left = false;
        turning_right = false;
        looking_up = false;
        looking_down = false;
    }
    
    bool IsMoving(){
        return left || right || up || down || forward || backward;
    }
    
    bool IsTurning(){
        return (turning_left && !turning_right) || (!turning_left && turning_right);
    }
    
    bool IsLooking(){
        return (looking_down && !looking_up) || (!looking_down && looking_up);
    }
    
    GeoVector ToVector(){
        
        int x = 0, y = 0, z = 0;
        if( left )
            x--;
        if( right )
            x++;
        if( up )
            y++;
        if( down )
            y--;
        if( forward )
            z++;
        if( backward )
            z--;
        return GeoVector( x, y, z ).Normalize();
    }
};




void MarkDirection( Direction& direction, const char key, const bool enabled ){
    switch ( key ){
        case SDLK_w: {
            direction.forward = enabled;
            break;
        }
            
        case SDLK_s: {
            direction.backward = enabled;
            break;
        }
            
        case SDLK_a: {
            direction.left = enabled;
            break;
        }
            
        case SDLK_d: {
            direction.right = enabled;
            break;
        }
            
        case SDLK_z: {
            direction.down = enabled;
            break;
        }
            
        case SDLK_x: {
            direction.up = enabled;
            break;
        }
            
        case SDLK_o: {
            direction.turning_left = enabled;
            break;
        }
            
        case SDLK_p: {
            direction.turning_right = enabled;
            break;
        }
            
        case SDLK_n: {
            direction.looking_down = enabled;
            break;
        }
            
        case SDLK_m: {
            direction.looking_up = enabled;
            break;
        }
            
            
    }
}




int main(int argc, char *argv[])
{
    TextureCache::ConfigureTexturePaths("/Users/mconway/projects/volley/images");
    ShaderCache::ConfigureShaderPaths("/Users/mconway/projects/volley/shaders/vertex", "/Users/mconway/projects/volley/shaders/pixel");
    
    SDL_Window* gWindow = NULL;
    SDL_GLContext gGlContext;
    
    SDL_version linked_sdl_version;
    SDL_GetVersion( &linked_sdl_version );
    printf( "linked sdl version is %i.%i.%i\n", linked_sdl_version.major, linked_sdl_version.minor, linked_sdl_version.patch );
    
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        std::cout << "SDL cannot init with error " << SDL_GetError() << std::endl;
        return -1;
    }
    
    
    // initialize controllers
    SDL_GameControllerAddMappingsFromFile("/Users/mconway/projects/volley/gamecontrollerdb.txt");
    
    auto num_available_joysticks = SDL_NumJoysticks();
    
    printf( "number of available joysticks is %i", num_available_joysticks );
    SDL_Joystick* joy = nullptr;
    
    if( num_available_joysticks ){
        joy = SDL_JoystickOpen(0);
        
        if (joy) {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickNameForIndex(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
        } else {
            printf("Couldn't open Joystick 0\n");
        }
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
    
    
    Direction direction;
    
    
    while (!quit)
    {
        /* CALCULATE ELAPSED TIME */
        
        auto old_time = time;
        time = SDL_GetTicks();
        
        Uint32 elapsed_milliseconds = 0;
        float elapsed_seconds = 0;
        if( old_time > 0 ){
            elapsed_milliseconds = time - old_time;
            elapsed_seconds = elapsed_milliseconds / 1000.0f;
        }
        
        
        
        
 
        
        /* HANDLE JOYSTICK INPUTS */
        
        
        
        
        auto x_move = SDL_JoystickGetAxis(joy, 0);
        auto y_move = SDL_JoystickGetAxis(joy, 1);
        
        
        auto x_move2 = SDL_JoystickGetAxis(joy, 2);
        auto y_move2 = SDL_JoystickGetAxis(joy, 5);
        
        float x_factor = 0, y2_factor = 0;
        if( abs(x_move) > 2000 )
            x_factor = (float) x_move / 32767.0f;
       
        
        if( abs(y_move) > 2000 )
            y2_factor = (float) -y_move2 / 32767.0f * 2.0f;
        
        
        /*
        auto cam = scene.GetCamera();
        auto side = cam.GetEyeDirection().ZeroY().Normalize().Cross( GeoVector(0,1,0,0));
        
        
        // handle rotation
        if( abs(x_move2) > 2000 ){
            auto x2_factor = -(float) x_move2 / 32767.0f;
            scene.GetCamera().Turn(GeoVector(0,1,0), x2_factor);
            
            
        }
        
        // handle movement
        if( abs(y_move) > 2000 ){
            auto y_factor = (float) -y_move / 32767.0f * 2.0f;
            scene.GetCamera().SetTargetView( cam.GetEyePosition() + cam.GetEyeDirection().ZeroY(), cam.GetFocusPosition() + cam.GetEyeDirection().ZeroY()  );
            scene.GetCamera().SetCameraSpeed( y_factor );
        } else {
            scene.GetCamera().Stop();
        }
        
        //printf( "xmove2 is %i and y move2 is %i\n", x_move2, y_move2 ); */
        
        
        
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            switch( sdlEvent.type ){
                case SDL_QUIT:
                    quit = true;
                    break;
                    
                case SDL_JOYAXISMOTION: {
                    break;
                }
                    
                case SDL_JOYBUTTONDOWN: {
                    printf( "joy button pressed..\n");
                    printf( "button pressed is %i\n", sdlEvent.jbutton.button );

                    break;
                }
                    
                case SDL_JOYBUTTONUP: {
                    printf( "joy button released..\n");
                    printf( "button released is %i\n", sdlEvent.jbutton.button );

                    break;
                }
                    
                case SDL_KEYDOWN:
                    switch (sdlEvent.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: {
                            quit = true;
                            break;
                        }
                        case SDLK_w:
                        case SDLK_s:
                        case SDLK_a:
                        case SDLK_d:
                        case SDLK_z:
                        case SDLK_x:
                        case SDLK_o:
                        case SDLK_p:
                        case SDLK_n:
                        case SDLK_m:{
                            MarkDirection(direction, sdlEvent.key.keysym.sym, true);
                            break;
                        }
                    }
                    
                    
                    break;
                    
                case SDL_KEYUP:
                    switch (sdlEvent.key.keysym.sym)
                    {
                        case SDLK_w:
                        case SDLK_s:
                        case SDLK_a:
                        case SDLK_d:
                        case SDLK_z:
                        case SDLK_x:
                        case SDLK_o:
                        case SDLK_p:
                        case SDLK_n:
                        case SDLK_m:{
                            MarkDirection(direction, sdlEvent.key.keysym.sym, false);
                            break;
                        }
                    }
                    
                    
                    break;
                    
                    
                
            }
        }
        
        auto cam = scene.GetCamera();
        auto fps_cam = dynamic_cast<FirstPersonCamera*>(cam);
        if( fps_cam ){
            if( direction.IsMoving() )
                fps_cam->Move( direction.ToVector().FlipZ() * elapsed_seconds );
            
            if( direction.IsTurning() ){
                if( direction.turning_left )
                    fps_cam->TurnLeft( elapsed_seconds * 100 );
                else if( direction.turning_right )
                    fps_cam->TurnRight( elapsed_seconds * 100 );
            }
            
            if( direction.IsLooking() ){
                if( direction.looking_down )
                    fps_cam->LookDown( elapsed_seconds * 72 );
                else if( direction.looking_up )
                    fps_cam->LookUp( elapsed_seconds * 72 );
            }
        }
        
      
        
        scene.Update( elapsed_seconds );
        
        // render stuff here
        scene.Draw();
        
        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }
    
    // clear resource
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    
    if (joy && SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }
    
    SDL_Quit();
    
    return 0;
}