#include "texture_cache.h"
#include "../../io/file_io.h"


std::string TextureCache::texture_path;

TextureCache::TextureCache(){
    
}

TextureCache::~TextureCache(){
    
}


void TextureCache::ConfigureTexturePaths( const std::string& t_path ){
    texture_path = t_path;
}



bool TextureCache::IsFileLoaded( const std::string& filepath ){
    return textures.find(filepath) != textures.end();
}


OpenGL::Texture TextureCache::FromFile( std::string filepath ){
    if( !IsFileLoaded(filepath)){
        auto resolved = ResolveTextureFilepath(filepath);
        RegisterTexture( filepath, OpenGL::Texture( resolved ) );
    }
  
    return textures[filepath];
}

OpenGL::Texture TextureCache::FromName( std::string name ){
    if( IsFilename( name ))
        return FromFile(name);
    return textures[name];
}

bool TextureCache::RegisterTexture( const std::string& name, OpenGL::Texture texture ){
    textures[name] = texture;
    return true;
}


std::string TextureCache::ResolveTextureFilepath( const std::string& filepath ){
    return File::PathJoin( texture_path, filepath );
}

bool TextureCache::IsFilename( const std::string& name ){
    return name.find(".") != std::string::npos;
}

