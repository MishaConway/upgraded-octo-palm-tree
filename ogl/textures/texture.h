#pragma once


#include <string>
#include <map>
#include <vector>
#include "glew.h"
#include "../../geo/GeoFloat.h"
#include "../../colors/color.h"



namespace OpenGL{
  
    enum TEXTURE_USAGE
    {
        shader_resource = 0,
        render_target
    };
    
    
    class GraphicsDevice;
    class Texture
    {
        friend class GraphicsDevice;
    public:
        Texture();
        Texture( const unsigned int width, const unsigned int height );
        Texture( const unsigned int width, const unsigned int height, const Color& color );
        Texture( const unsigned int width, const unsigned int height,  const TEXTURE_USAGE usage );
        Texture( const std::string& image_filename );
        Texture( std::vector<Texture> textures );
        Texture Clone( const unsigned width, const unsigned int height );
        GLint GetOpenGLTextureId();
        GLenum GetFormat();
        bool IsTextureArray();
        bool IsFloatTexture();
        bool SaveToFile( const std::string& filename, const bool save_only_once = false );
        unsigned char* Map( unsigned int* pPitch );
        void Unmap();
    protected:
        void PrepareTextureArray( std::vector<Texture> textures );
        void Setup( const unsigned int width, const unsigned int height, const TEXTURE_USAGE usage );
    private:
        GLuint texture_id, pbo_id;
        GLenum format;
        GLenum internal_format, external_format;
        GLenum component_type;
        unsigned char* pMappedData;
    };
}
