#pragma once

#include <vector>
#include "glew.h"
#include <SDL2/SDL_opengl.h>
#include "../../interfaces/IValidatable.h"


namespace OpenGL{
    class Buffer : public IValidatable
    {
    public:
        Buffer();
        Buffer( void* pData, const unsigned int data_size, GLenum buffer_type, GLenum usage );
        void* Map();
        bool Unmap();
        bool SetData( void* pData  );
        GLuint GetOpenGLBufferId();
    protected:
        GLuint vbo_id;
        GLenum vbo_buffer_type, vbo_usage, vbo_access;
        unsigned int data_size;
    };
}
