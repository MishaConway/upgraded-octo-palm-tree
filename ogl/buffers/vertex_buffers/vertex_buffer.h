#pragma once
#include "../buffer.h"
#include "../index_buffers/index_buffer.h"


namespace OpenGL{
    enum PRIMITIVE_TYPE
    {
        POINTLIST	= 1,
        LINELIST	= 2,
        LINESTRIP	= 3,
        TRIANGLELIST	= 4,
        TRIANGLESTRIP	= 5
    };
    
    template<typename T>
    class VertexBuffer
    {
    public:
        VertexBuffer(){
            num_vertices = 0;
            indexed = false;
        }
        
        VertexBuffer( const std::vector<T>& vertices ){
            indexed = false;
            InitializeVertexBuffer( vertices, PRIMITIVE_TYPE::TRIANGLELIST, GL_STATIC_DRAW );
        }
        
        
        VertexBuffer( const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type ){
            indexed = false;
            InitializeVertexBuffer( vertices, primitive_type, GL_STATIC_DRAW );
        }
        
      
        VertexBuffer( const std::vector<unsigned int>& indices, const std::vector<T>& vertices ){
            indexed = true;
            index_buffer = IndexBuffer(indices);
            InitializeVertexBuffer( vertices, PRIMITIVE_TYPE::TRIANGLELIST, GL_STATIC_DRAW );
        }
        
        VertexBuffer( const std::vector<unsigned short>& indices, const std::vector<T>& vertices ){
            indexed = true;
            index_buffer = IndexBuffer(indices);
            InitializeVertexBuffer( vertices, PRIMITIVE_TYPE::TRIANGLELIST, GL_STATIC_DRAW );
        }
        
        VertexBuffer( const std::vector<unsigned int>& indices, const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type  ){
            indexed = true;
            index_buffer = IndexBuffer(indices);
            InitializeVertexBuffer( vertices, primitive_type, GL_STATIC_DRAW );
        }
        
        VertexBuffer( const std::vector<unsigned short>& indices, const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type  ){
            indexed = true;
            index_buffer = IndexBuffer(indices);
            InitializeVertexBuffer( vertices, primitive_type, GL_STATIC_DRAW );
        }
        
        PRIMITIVE_TYPE GetPrimitiveType(){
            return primitive_type;
        }
        
        GLenum GetOpenGLPrimitiveType()
        {
            
            switch( primitive_type )
            {
                case PRIMITIVE_TYPE::TRIANGLELIST:
                    return GL_TRIANGLES;
                case PRIMITIVE_TYPE::LINELIST:
                    return GL_LINES;
                case PRIMITIVE_TYPE::POINTLIST:
                    return GL_POINTS;
                case PRIMITIVE_TYPE::LINESTRIP:
                    return GL_LINE_STRIP;
                case PRIMITIVE_TYPE::TRIANGLESTRIP:
                    return GL_TRIANGLE_STRIP;
            }
            
            
            return -1;
        }
        
        virtual bool IsValid()
        {
            return buffer.IsValid() && (!indexed || index_buffer.IsValid());
        }
        
        bool IsIndexed(){
            return indexed;
        }
        
        unsigned int GetNumVertices(){
            return num_vertices;
        }
        
        unsigned int GetNumIndices(){
            return index_buffer.GetNumIndices();
        }
        
        bool Bind(){
            glBindBuffer(GL_ARRAY_BUFFER, buffer.GetOpenGLBufferId() );
            if( indexed )
                index_buffer.Bind();
            return true;
        }
        
        bool UnBind(){
            glBindBuffer(GL_ARRAY_BUFFER, 0 );
            if( indexed )
                index_buffer.UnBind();
            return true;
        }
        
        bool Draw(){
            Bind();
            if( indexed ){
                glDrawElements( GetOpenGLPrimitiveType(), GetNumIndices(), index_buffer.GetIndexType(), 0 );
            } else {
                glDrawArrays( GetOpenGLPrimitiveType(), 0, GetNumVertices() );
            }
            UnBind();
            return true;
        }
        
    protected:
        void InitializeVertexBuffer( const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type, const GLenum usage ){
            this->primitive_type = primitive_type;
            num_vertices = (unsigned int) vertices.size();
            if( !vertices.empty() ){
                void* pInitialData = (void*) &vertices[0];
                buffer = Buffer( pInitialData, num_vertices * sizeof(T), GL_ARRAY_BUFFER, usage );
            }
            
        }
    protected:
        Buffer buffer;
        PRIMITIVE_TYPE primitive_type;
        unsigned int num_vertices;
        
        bool indexed;
        IndexBuffer index_buffer;
    };
}
