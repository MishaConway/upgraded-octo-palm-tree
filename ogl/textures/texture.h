#pragma once


#include <string>
#include <map>
#include <vector>
#include <exception>
#include "glew.h"
#include "../../interfaces/IValidatable.h"
#include "../../interfaces/IHasErrors.h"
#include "../../geo/GeoFloat.h"
#include "../../colors/color.h"



namespace OpenGL{
  
    enum TEXTURE_USAGE
    {
        TEXTURE_USAGE_SHADER_RESOURCE = 1 << 0,
        TEXTURE_USAGE_RENDER_TARGET = 1 << 1,
        TEXTURE_USAGE_FLOAT = 1 << 2,
        TEXTURE_USAGE_CUBEMAP = 1 << 3
    };
    typedef uint8_t TextureUsageMask;

    
    enum CUBEMAP_SIDE{
        CUBEMAP_POSITIVE_X = 0,
        CUBEMAP_NEGATIVE_X,
        CUBEMAP_POSITIVE_Y,
        CUBEMAP_NEGATIVE_Y,
        CUBEMAP_POSITIVE_Z,
        CUBEMAP_NEGATIVE_Z,
        CUBEMAP_NONE
    };
    
    struct CubemapUnknownSideMapException : public std::exception
    {
        const char * what () const throw ()
        {
            return "Mapped a cubemap for writing without specifying a side";
        }
    };
    
    GLenum ToOpenGLCubeMapSide( const CUBEMAP_SIDE cubemap_side );


    class Texture : public IValidatable
    {
    public:
        Texture();
        Texture( const std::string& image_filename );
        Texture( const unsigned int width, const unsigned int height );
        Texture( const unsigned int width, const unsigned int height,  const TextureUsageMask usage );
        bool Free();

        GLint GetOpenGLTextureId();
        GLenum GetFormat();
        bool IsFloatTexture();
        bool IsCubeMap();
        bool SaveToFile( const std::string& filename );
        unsigned char* Map( unsigned int* pPitch );
        unsigned char* Map( unsigned int* pPitch, const CUBEMAP_SIDE side );

        void Unmap();
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
    protected:
        void Setup( const unsigned int width, const unsigned int height, const TextureUsageMask usage );
        unsigned char* MapEx( unsigned int* pPitch );
        
    private:
        GLuint texture_id, pbo_id;
        GLenum internal_format, format, component_type, enable_target;

        unsigned int width, height, bpp;
        
        unsigned char* pMappedData;
        CUBEMAP_SIDE mapped_side;
        
        TextureUsageMask usage;
    };
}
