#pragma once

#include <map>
#include "../../ogl/textures/texture.h"




class TextureCache{
public:
    TextureCache();
    ~TextureCache();
    static void ConfigureTexturePaths( const std::string& texture_path );
    OpenGL::Texture FromFile( std::string name );
    bool IsFileLoaded( const std::string& filepath );
protected:
    std::string ResolveTextureFilepath( const std::string& filepath );
    
    std::map< std::string, OpenGL::Texture> textures;    
    static std::string texture_path;
};