#include "texture.h"
#include "../device/device.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


OpenGL::Texture::Texture(){
    valid = false;
}

OpenGL::Texture::Texture( const std::string& image_filename )
{
    valid = false;
    
    glEnable(GL_TEXTURE_2D);
    
    int image_width, image_height, image_bpp;
    unsigned char* image_data = stbi_load(image_filename.c_str(), &image_width, &image_height, &image_bpp, STBI_rgb);
    
    texture_id = 3;//SOIL_load_OGL_texture( image_filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,  SOIL_FLAG_MIPMAPS );
    if( !texture_id )
    {
        //printf( "soil failed to load %s\n", image_filename.c_str() );
        //printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    
    printf( "image_width is %i\n", image_width);
    printf( "image_height is %i\n", image_height);
    printf( "image_bpp is %i\n", image_bpp);
    printf( "image filename is %s\n", image_filename.c_str() );



    Setup(image_width, image_width, TEXTURE_USAGE::SHADER_RESOURCE );
    
    unsigned char* pMappedBytes = Map(0);
    memcpy( pMappedBytes, image_data, width * height * 3);
    Unmap();
    
    stbi_image_free( image_data );
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
    external_format = GL_RGB;
    component_type = GL_FLOAT;
    
    if( GL_FLOAT == component_type ){
        internal_format = external_format;
        //internal_format = GL_RGBA32F;

        bpp = 3;//sizeof(float)*4;
    }
    else{
        internal_format = GL_RGB;
        bpp = 3;
    }
    
    this->width = width;
    this->height = height;
    format = GL_RGB;
    
    glEnable( GL_TEXTURE_2D );
    glGenTextures(1, &texture_id );
    
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject() )
        glGenBuffers( 1, &pbo_id  );
    
    printf( "texture id is %i\n", texture_id);
    
    //bind our created opengl texture object so we may define its parameters
    glBindTexture( GL_TEXTURE_2D, texture_id  );
    
    if( TEXTURE_USAGE::RENDER_TARGET == usage  ){
        internal_format = external_format = GL_RGBA;
        component_type = GL_UNSIGNED_BYTE;

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else{
       /* glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap( GL_TEXTURE_2D ); */
    }
    
    //Upload the textel buffer using Pixel Buffer Objects
    if( GraphicsDevice::GetCapabilities().SupportsPixelBufferObject()  ){
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, pbo_id );
        glBufferData( GL_PIXEL_UNPACK_BUFFER, width*height*bpp, 0, GL_STREAM_DRAW );
        glTexImage2D( GL_TEXTURE_2D, 0, internal_format, width, height, 0, external_format, component_type, (const GLubyte*) 0 );
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    }
    else{
       // glTexImage2D( GL_TEXTURE_2D, 0, internal_format, width, height, 0, external_format, component_type, (const GLubyte*) 0 );
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
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, external_format, component_type, 0 );
        
        //Unbind our Pixel Buffer Object
        glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );
    }
    else
    {
        //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, external_format, component_type, pMappedData );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pMappedData);

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
