#include "texture.h"
#include "../device/device.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


OpenGL::Texture::Texture(){
    valid = false;
}

OpenGL::Texture::Texture( const std::string& image_filename )
{
    valid = false;
    
    glEnable(GL_TEXTURE_2D);
    
    int image_width, image_height, image_bpp;
    unsigned char* start_image_data = stbi_load(image_filename.c_str(), &image_width, &image_height, &image_bpp, STBI_rgb_alpha);
    
    //printf( "image filename is %s\n", image_filename.c_str() );
    //printf( "image_width is %i\n", image_width);
    ///printf( "image_height is %i\n", image_height);
    //printf( "image_bpp is %i\n", image_bpp);



    Setup(image_width, image_height, TEXTURE_USAGE::SHADER_RESOURCE );
    
    
    unsigned char* pMappedBytes = Map(0);
    
    // it seems like stbi load with STBI_rgb_alpha fills in alpha channel?
    memcpy( pMappedBytes, start_image_data, width * height * bpp);

    /*
    
    if( 4 == image_bpp){
        memcpy( pMappedBytes, start_image_data, width * height * 4);
    } else if( 3 == image_bpp ){
        auto image_data = start_image_data;
        for( int i = 0; i < width * height; i++ ){
            *pMappedBytes++ = *image_data++; //red
            *pMappedBytes++ = *image_data++; //green
            *pMappedBytes++ = *image_data++; //blue
            *pMappedBytes++ = 255;           //alpha
        }
    } */
    Unmap();
    
    stbi_image_free( start_image_data );
}

OpenGL::Texture::Texture( const unsigned int width, const unsigned int height,  const TEXTURE_USAGE usage ){
    Setup( width, height, usage );
}

OpenGL::Texture::Texture( const unsigned int width, const unsigned int height )
{
    Setup( width, height, TEXTURE_USAGE::SHADER_RESOURCE );
}

bool OpenGL::Texture::IsFloatTexture()
{
    return component_type == GL_FLOAT;
}


void OpenGL::Texture::Setup( const unsigned int width, const unsigned int height, const TEXTURE_USAGE usage  )
{
    this->width = width;
    this->height = height;
    this->bpp = 4;
    component_type = GL_UNSIGNED_BYTE;
    internal_format = GL_RGBA8;
    format = GL_RGBA;

    glEnable( GL_TEXTURE_2D );
    glGenTextures(1, &texture_id );
    
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject() )
        glGenBuffers( 1, &pbo_id  );
    
    
    //bind our created opengl texture object so we may define its parameters
    glBindTexture( GL_TEXTURE_2D, texture_id  );
    
    if( TEXTURE_USAGE::RENDER_TARGET == usage  ){
        internal_format = format = GL_RGBA;
        component_type = GL_UNSIGNED_BYTE;

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else{
        //trilinear min filtering
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        float aniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

unsigned char* OpenGL::Texture::Map( unsigned int* pPitch )
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, texture_id  );
    
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
    //Bind our Pixel Buffer Object and attempt to unmap it...
    glBindTexture( GL_TEXTURE_2D, texture_id  );
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject() )
    {
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, pbo_id );
        glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, component_type, 0 );
        
        //Unbind our Pixel Buffer Object
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, component_type, pMappedData);
        delete [] pMappedData;
        pMappedData = nullptr;
    }
    glGenerateMipmap( GL_TEXTURE_2D );
}

bool OpenGL::Texture::ClearColor( Color color, const bool preserve_alpha )
{
    float red = color.GetNormalizedRed();
    float green = color.GetNormalizedGreen();
    float blue = color.GetNormalizedBlue();
    float alpha = color.GetNormalizedAlpha();
    
    unsigned char* pMappedBytes = Map(0);
    for( int y = 0; y < height; y++ )
        for( int x = 0; x < width; x++ ){
            
        }
    
    Unmap();
    
    return true;
}

bool OpenGL::Texture::SaveToFile( const std::string& filename )
{
    std::string extension = filename.substr(filename.find_last_of(".")+1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    GLint is_compressed = GL_FALSE;
    GLint compressed_size = 0;
    GLint compressed_format = 0;
    glBindTexture( GL_TEXTURE_2D, texture_id  );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &is_compressed );
    if( is_compressed )
    {
        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size );
        glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compressed_format );
        printf( "sorry can't save compressed textures yet...\n" );
        return false;
    }
    else
    {
        unsigned char* pixels = new unsigned char[ width*height*bpp ];
        glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
        stbi_write_jpg("/Users/mconway/projects/volley/test.jpg", width, height, bpp, pixels, 0);
        
        delete [] pixels;
    }
    
    return true;
}





/*
 
 GLuint TextureManager::LoadTexture(std::string filename, bool useMipMaps, GLuint texture_unit, GLuint   wrapflag, bool pixelate)
 {
 itor = textures.find(filename); //lookup this texture in our std::map
 
 if(itor == textures.end())
 {
 //we didn't find that texture name, so it is a new texture
 tex *newtex = new tex;
 
 newtex->refcount = 1;
 newtex->unload = true; //currently setting all textures to unload when refcount = 0;
 
 //add the path to the file
 std::string fullpath;
 fullpath = texturePath;
 fullpath.append(filename);
 
 //pointer to the image data
 BYTE* bits(0);
 //image width and height, and #of components (1= gray scale, 4 = rgba)
 int width(0), height(0), components(0);
 //OpenGL's image ID to map to
 GLuint gl_texID;
 
 //retrieve the image data, currently force to RGBA (4 components)
 bits = stbi_load(filename.c_str(), &width, &height, &components, 4);
 
 //if somehow one of these failed (they shouldn't), return failure
 if((bits == 0) || (width == 0) || (height == 0))
 {
 if(bits == 0) tLog(Level::Severe) << "bits = 0";
 if(width == 0) tLog(Level::Severe) << "width = 0";
 if(height == 0) tLog(Level::Severe) << "height = 0";
 goto ERROR_HANDLER;
 }
 
 //generate an OpenGL texture ID for this texture
 glGenTextures(1, &gl_texID);
 //store the texture ID mapping
 newtex->texId = gl_texID;
 
 glActiveTexture(texture_unit); //needed for programmable shaders?
 //bind to the new texture ID
 glBindTexture(GL_TEXTURE_2D, gl_texID);
 
 //set up some vars for OpenGL texturizing
 GLenum image_format = GL_RGBA;
 GLint internal_format = GL_RGBA;
 GLint level = 0;
 //store the texture data for OpenGL use
 glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
 0, image_format, GL_UNSIGNED_BYTE, bits);
 
 //swizzle colors - not needed for stb_image
 //GLint swizzleMask[] = { GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA };
 //glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
 
 if (useMipMaps)
 {
 glGenerateMipmap(GL_TEXTURE_2D);
 }
 
 //Free stb's copy of the data
 stbi_image_free(bits);
 
 newtex->width = width;
 newtex->height = height;
 
 //add the new texture to the map
 textures[filename] = newtex;
 
 currentId[texture_unit - GL_TEXTURE0] = newtex->texId;
 
 //setup texture filtering for when we are close/far away
 if (useMipMaps)
 {
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //for when we are close
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//when we are far away
 }
 else if(pixelate)
 {
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //for when we are close
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//when we are far away
 }
 else
 {
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //for when we are close
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//when we are far away
 }
 
 
 //the following turns on a special, high-quality filtering mode called "ANISOTROPY"
 if(GL_EXT_texture_filter_anisotropic)
 {
 GLfloat largest_supported_anisotropy;
 glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
 }
 
 // the texture stops at the edges with GL_CLAMP_TO_EDGE
 //...experiment with GL_CLAMP and GL_REPEAT as well
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapflag );
 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapflag );
 
 //return the loaded texture object
 return newtex->texId;
 }
 
 //if we get here in the code, we already had that texture loaded by some other object
 (*itor->second).refcount++; //update the reference counter
 //bind it to the texture unit
 BindTexture((*itor->second).texId, texture_unit);
 return (*itor->second).texId;//and return the OpenGL texture object ID associated with that texture
 
 ERROR_HANDLER:
 tLog(Level::Severe) << "ERROR loading file: " << filename;
 assert(false && "ERROR loading file");
 return 0;
 }
 */
