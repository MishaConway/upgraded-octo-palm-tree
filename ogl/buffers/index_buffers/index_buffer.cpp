#include "index_buffer.h"

OpenGL::IndexBuffer::IndexBuffer(){
    num_indices = 0;
}

OpenGL::IndexBuffer::IndexBuffer( const std::vector<unsigned int>& indices ){
    InitializeIndexBuffer( (void*) &indices[0], (unsigned int) indices.size(), sizeof(unsigned int), GL_UNSIGNED_INT  );
}

OpenGL::IndexBuffer::IndexBuffer( const std::vector<unsigned short>& indices ){
    InitializeIndexBuffer( (void*) &indices[0], (unsigned int) indices.size(), sizeof(unsigned short),  GL_UNSIGNED_SHORT );
}

bool OpenGL::IndexBuffer::IsValid()
{
    return buffer.IsValid();
}

unsigned int OpenGL::IndexBuffer::GetNumIndices(){
    return num_indices;
}

GLenum OpenGL::IndexBuffer::GetIndexType(){
    return index_type;
}

bool OpenGL::IndexBuffer::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.GetOpenGLBufferId() );
    return true;
}

bool OpenGL::IndexBuffer::UnBind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
    return true;
}


void OpenGL::IndexBuffer::InitializeIndexBuffer( void* pInitialData, const unsigned int num_indices, const unsigned int index_size, const GLenum index_type ){
    this->num_indices = num_indices;
    this->index_type = index_type;
    if( num_indices )
        buffer = Buffer( pInitialData, num_indices * index_size, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW );
}