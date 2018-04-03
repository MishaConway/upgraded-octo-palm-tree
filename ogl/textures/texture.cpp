#include "texture.h"
#include "../device/device.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


OpenGL::Texture::Texture(){
    valid = false;
    mapped_side = CUBEMAP_SIDE::CUBEMAP_NONE;
}

OpenGL::Texture::Texture( const std::string& image_filename )
{
    valid = false;
    enable_target = GL_TEXTURE_2D;
    
    int image_width, image_height, image_bpp;
    unsigned char* start_image_data = stbi_load(image_filename.c_str(), &image_width, &image_height, &image_bpp, STBI_rgb_alpha);
    
    Setup(image_width, image_height, TEXTURE_USAGE_SHADER_RESOURCE );
    
    unsigned char* pMappedBytes = Map(0);
    memcpy( pMappedBytes, start_image_data, width * height * bpp);
    Unmap();
    
    stbi_image_free( start_image_data );
}

OpenGL::Texture::Texture( const unsigned int width, const unsigned int height )
{
    enable_target = GL_TEXTURE_2D;
    Setup( width, height, TEXTURE_USAGE_SHADER_RESOURCE );
}

OpenGL::Texture::Texture( const unsigned int width, const unsigned int height,  const TextureUsageMask usage ){
    enable_target = GL_TEXTURE_2D;
    Setup( width, height, usage );
}

bool OpenGL::Texture::Free(){
    glDeleteTextures( 1, &texture_id );
    valid = false;
    return true;
}


bool OpenGL::Texture::IsFloatTexture(){
    return component_type == GL_FLOAT;
}

bool OpenGL::Texture::IsCubeMap(){
    return usage & TEXTURE_USAGE_CUBEMAP;
}


void OpenGL::Texture::Setup( const unsigned int width, const unsigned int height, const TextureUsageMask usage  )
{
    this->usage = usage;
    this->width = width;
    this->height = height;
    this->bpp = 4;
    mapped_side = CUBEMAP_SIDE::CUBEMAP_NONE;
    component_type = GL_UNSIGNED_BYTE;
    internal_format = format = GL_RGBA;
    if( usage & TEXTURE_USAGE_FLOAT  ){
        component_type = GL_FLOAT;
        internal_format = GL_RGBA16F;
    }

    glEnable( enable_target );
    glGenTextures(1, &texture_id );
 

    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject() )
        glGenBuffers( 1, &pbo_id  );
    
    
    //bind our created opengl texture object so we may define its parameters
    glBindTexture( enable_target, texture_id  );
    
    if( usage & TEXTURE_USAGE_CUBEMAP ){
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        for( int i = 0 ; i < 6; i++ )
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, internal_format, width, height, 0, format, component_type, 0);
        
    } else {
        
        if( usage & TEXTURE_USAGE_RENDER_TARGET    ){
            glTexParameteri( enable_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(enable_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, component_type, 0);
        }
        else{
            //trilinear min filtering
            glTexParameterf(enable_target,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf(enable_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            float aniso = 0.0f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
            glTexParameterf(enable_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
            
            glTexParameteri(enable_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(enable_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }
    
    
    
   
    
    valid = true;
}



GLint OpenGL::Texture::GetOpenGLTextureId()
{
    return texture_id;
}

GLenum OpenGL::Texture::GetFormat()
{
    return format;
}

unsigned char* OpenGL::Texture::Map( unsigned int* pPitch, const CUBEMAP_SIDE side ){
    mapped_side = side;
    return MapEx( pPitch );
}


unsigned char* OpenGL::Texture::Map( unsigned int* pPitch ){
    mapped_side = CUBEMAP_SIDE::CUBEMAP_NONE;
    return MapEx( pPitch );
}

unsigned char* OpenGL::Texture::MapEx( unsigned int* pPitch ){
    glEnable(enable_target);
    glBindTexture( enable_target, texture_id  );
    
    pMappedData = nullptr;
    const unsigned int buffer_size = width*height*bpp;
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject()  )
    {
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_id );
        
        //we can quickly invalidate the data stored on the GPU by nulling it
        //before mapping the buffer to the client side. This allows the driver to avoid stalls due to
        //attempting to maintain a copy of the old textel buffer.
        glBufferData(GL_PIXEL_UNPACK_BUFFER, buffer_size, (const GLubyte*) 0, GL_DYNAMIC_DRAW);
        
        //Map our Pixel Buffer Object into client side memory
        pMappedData = (unsigned char* )glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        
        //Unbind our Pixel Buffer Object
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    }
    else
        pMappedData = new unsigned char[buffer_size];
    
    if( pPitch )
        *pPitch = width * bpp;
    return pMappedData;
}

void OpenGL::Texture::Unmap()
{
    GLenum texture_target = GL_TEXTURE_2D;
    if( IsCubeMap() )
        texture_target = ToOpenGLCubeMapSide( mapped_side );
    
    
    //Bind our Pixel Buffer Object and attempt to unmap it...
    glBindTexture( enable_target, texture_id  );
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject() )
    {
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, pbo_id );
        glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );
        glTexSubImage2D(texture_target, 0, 0, 0, width, height, format, component_type, 0 );
        
        //Unbind our Pixel Buffer Object
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    }
    else
    {
        glTexImage2D(texture_target, 0, internal_format, width, height, 0, format, component_type, pMappedData);
        delete [] pMappedData;
        pMappedData = nullptr;
    }
    glGenerateMipmap( enable_target );
}


/*
bool OpenGL::Texture::SaveToFile( const std::string& filename )
{
    std::string extension = filename.substr(filename.find_last_of(".")+1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    GLint is_compressed = GL_FALSE;
    GLint compressed_size = 0;
    GLint compressed_format = 0;
    glBindTexture( enable_target, texture_id  );
    glGetTexLevelParameteriv( texture_target, 0, GL_TEXTURE_COMPRESSED, &is_compressed );
    if( is_compressed )
    {
        glGetTexLevelParameteriv( enable_target, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size );
        glGetTexLevelParameteriv( enable_target, 0, GL_TEXTURE_INTERNAL_FORMAT, &compressed_format );
        printf( "sorry can't save compressed textures yet...\n" );
        return false;
    }
    else
    {
        unsigned char* pixels = new unsigned char[ width*height*bpp ];
        glGetTexImage( texture_target, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
        stbi_write_jpg("/Users/mconway/projects/volley/test.jpg", width, height, bpp, pixels, 0);
        
        delete [] pixels;
    }
    
    return true;
}*/


unsigned int OpenGL::Texture::GetWidth() const{
    return width;
}

unsigned int OpenGL::Texture::GetHeight() const{
    return height;
}

GLenum OpenGL::Texture::ToOpenGLCubeMapSide( const CUBEMAP_SIDE cubemap_side ){
    switch( cubemap_side ){
        case CUBEMAP_SIDE::CUBEMAP_NEGATIVE_X:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case CUBEMAP_SIDE::CUBEMAP_POSITIVE_X:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case CUBEMAP_SIDE::CUBEMAP_NEGATIVE_Y:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        case CUBEMAP_SIDE::CUBEMAP_POSITIVE_Y:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case CUBEMAP_SIDE::CUBEMAP_NEGATIVE_Z:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case CUBEMAP_SIDE::CUBEMAP_POSITIVE_Z:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    }
    return -1;
}





