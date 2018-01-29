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
    auto s = ResolveTextureFilepath(filepath);
    return textures.find(ResolveTextureFilepath(filepath)) != textures.end();
}


OpenGL::Texture TextureCache::FromFile( std::string filepath ){
    auto resolved = ResolveTextureFilepath(filepath);

    if( !IsFileLoaded(filepath)){
        textures[resolved] = OpenGL::Texture( resolved );
    }
  
    return textures[resolved];
}

std::string TextureCache::ResolveTextureFilepath( const std::string& filepath ){
    return File::PathJoin( texture_path, filepath );
}
