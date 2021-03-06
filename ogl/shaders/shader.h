#pragma once

#include "glew.h"
#include <map>
#include <string>
#include "../../interfaces/IValidatable.h"
#include "../../interfaces/IHasErrors.h"


namespace OpenGL{
    class Shader : public IValidatable, public IHasErrors
    {
    public:
        GLuint GetOpenGLShaderId();
        bool Free();
    protected:
        Shader( GLenum shader_type, std::string source );
    protected:
        GLuint shader_id;
    };
    
    class VertexShader : public Shader{
    public:
        VertexShader( std::string source );
    };
    
    class PixelShader : public Shader{
    public:
        PixelShader( std::string source );
    };
}

