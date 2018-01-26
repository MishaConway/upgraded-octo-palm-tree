#pragma once


#include "shader_cache/shader_cache.h"
#include "camera/camera.h"


class Scene{
public:
    void Initialize();
    void Draw();
    
    
    
    
protected:
    
    ShaderCache shader_cache;
    Camera camera;
    
    
    
};