#pragma once

#include <map>
#include "../../ogl/textures/texture.h"




class TextureCache{
public:
    TextureCache();
    ~TextureCache();
    static void ConfigureTexturePaths( const std::string& texture_path );
    OpenGL::Texture FromFile( std::string name );
    OpenGL::Texture FromName( std::string name );
    bool IsFileLoaded( const std::string& filepath );
    bool RegisterTexture( const std::string& name, OpenGL::Texture texture );
protected:
    std::string ResolveTextureFilepath( const std::string& filepath );
    bool IsFilename( const std::string& name );
    
    std::map< std::string, OpenGL::Texture> textures;    
    static std::string texture_path;
};