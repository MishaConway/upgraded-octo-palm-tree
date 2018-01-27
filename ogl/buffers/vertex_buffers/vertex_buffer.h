#pragma once
#include "../buffer.h"


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
        }
        
        VertexBuffer( const std::vector<T>& vertices ){
            InitializeVertexBuffer( vertices, PRIMITIVE_TYPE::TRIANGLELIST, GL_STATIC_DRAW );
        }
        
        
        VertexBuffer( const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type ){
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
            return buffer.IsValid();
        }
        
        unsigned int GetNumVertices(){
            return num_vertices;
        }
        
        bool Bind(){
            glBindBuffer(GL_ARRAY_BUFFER, buffer.GetOpenGLBufferId() );
            return true;
        }
        
        bool Draw(){
            glBindBuffer(GL_ARRAY_BUFFER, buffer.GetOpenGLBufferId() );
            glDrawArrays( GetOpenGLPrimitiveType(), 0, GetNumVertices() );
            return true;
        }
        
    protected:
        void InitializeVertexBuffer( const std::vector<T>& vertices, const PRIMITIVE_TYPE primitive_type, const GLenum usage ){
            this->primitive_type = primitive_type;
            num_vertices = vertices.size();
            if( !vertices.empty() ){
                void* pInitialData = (void*) &vertices[0];
                buffer = Buffer( pInitialData, num_vertices * sizeof(T), GL_ARRAY_BUFFER, usage );
            }
            
        }
    protected:
        Buffer buffer;
        PRIMITIVE_TYPE primitive_type;
        unsigned int num_vertices;
    };
}
