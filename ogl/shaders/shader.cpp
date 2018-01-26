#include "shader.h"
#include <SDL2/SDL_opengl.h>



OpenGL::Shader::Shader( const GLenum shader_type, std::string source )
{
    valid = false;
    
    if( !source.empty() ){
        shader_id = glCreateShader( shader_type );
            
        const char* c_str = source.c_str();
        const GLint length = (const GLint) source.length();
        glShaderSource( shader_id, 1, &c_str, &length );
        glCompileShader(shader_id);
            
        GLint compiled;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
        if (compiled){
            valid = true;
        }
        else {
            GLint blen = 0;
            GLsizei slen = 0;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH , &blen);
            if (blen > 1) {
                GLchar* compiler_log = new GLchar[blen];
                glGetShaderInfoLog(shader_id, blen, &slen, compiler_log);
                errors = std::string( compiler_log );
                delete [] compiler_log;

            }
        }
    }
}

GLuint OpenGL::Shader::GetOpenGLShaderId()
{
    return shader_id;
}


OpenGL::VertexShader::VertexShader( std::string source  ) : OpenGL::Shader( GL_VERTEX_SHADER, source ){}
OpenGL::PixelShader::PixelShader( std::string source  ) : OpenGL::Shader( GL_FRAGMENT_SHADER, source ){}