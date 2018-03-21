#pragma once
#include "../buffer.h"


namespace OpenGL{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        IndexBuffer( const std::vector<unsigned int>& indices );
        IndexBuffer( const std::vector<unsigned short>& indices );
        
        virtual bool IsValid();
        
        unsigned int GetNumIndices();
        GLenum GetIndexType();
        
        bool Bind();
        bool UnBind();
    protected:
        void InitializeIndexBuffer( void* pInitialData, const unsigned int num_indices, const unsigned int index_size, const GLenum index_type );
    protected:
        Buffer buffer;
        unsigned int num_indices;
        GLenum index_type;
    };
}