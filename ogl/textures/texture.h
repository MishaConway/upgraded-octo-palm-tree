#pragma once


#include <string>
#include <map>
#include <vector>
#include "glew.h"
#include "../../interfaces/IValidatable.h"
#include "../../geo/GeoFloat.h"
#include "../../colors/color.h"



namespace OpenGL{
  
    enum TEXTURE_USAGE
    {
        SHADER_RESOURCE = 0,
        RENDER_TARGET
    };
    

    class Texture : public IValidatable
    {
    public:
        Texture();
        Texture( const std::string& image_filename );
        Texture( const unsigned int width, const unsigned int height );
        Texture( const unsigned int width, const unsigned int height,  const TEXTURE_USAGE usage );
        GLint GetOpenGLTextureId();
        GLenum GetFormat();
        bool IsFloatTexture();
        bool SaveToFile( const std::string& filename );
        bool ClearColor( Color color, const bool preserve_alpha );
        unsigned char* Map( unsigned int* pPitch );
        void Unmap();
    protected:
        void Setup( const unsigned int width, const unsigned int height, const TEXTURE_USAGE usage );

    private:
        GLuint texture_id, pbo_id;
        unsigned int width, height, bpp;
        GLenum internal_format, format;
        GLenum component_type;
        unsigned char* pMappedData;
    };
}
