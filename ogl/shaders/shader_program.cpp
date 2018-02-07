#include <algorithm>
#include "shader_program.h"
#include "../../string/string_utils.h"

OpenGL::ShaderAttributeDetails::ShaderAttributeDetails( std::string name, std::string type){
    this->name = name;
    this->type = type;
}

OpenGL::ShaderUniformDetails::ShaderUniformDetails(){}
OpenGL::ShaderUniformDetails::ShaderUniformDetails( GLint size, GLenum type ){
    this->size = size;
    this->type = type;
}


OpenGL::ShaderProgram::ShaderProgram(  const std::string& vertex_shader_source, const std::string& pixel_shader_source  ){
    valid = false;
    
    
    VertexShader vertex_shader( vertex_shader_source );
    PixelShader pixel_shader( pixel_shader_source );
    
    
    if( vertex_shader.IsValid() && pixel_shader.IsValid() )
    {
        program_id = glCreateProgram();
        glAttachShader( program_id, vertex_shader.GetOpenGLShaderId() );
        glAttachShader( program_id, pixel_shader.GetOpenGLShaderId() );
        
        auto regex_matches = GetRegexMatches( vertex_shader_source, "attribute([^;]+)" );
        for( unsigned int i = 0; i < regex_matches.size(); i++ ){
            auto attribute_line = ExplodeString( regex_matches[i], " \t\n" );
            auto attribute_name = attribute_line[2];
            auto attribute_type = attribute_line[1];
            input_attributes.push_back( ShaderAttributeDetails( attribute_name, attribute_type ) );
            printf( "binding attribute %s to slot %i\n", attribute_name.c_str(), i );
            glBindAttribLocation(program_id, i, attribute_name.c_str() );
        }
        
        glLinkProgram( program_id );
        GLint linked;
        glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
        if (linked)
        {
            valid = true;
            int total = -1;
            glGetProgramiv( program_id, GL_ACTIVE_UNIFORMS, &total );
            for(int i=0; i<total; ++i)
            {
                int name_len=-1;
                GLint num =-1;
                GLenum type = GL_ZERO;
                char name[256];
                glGetActiveUniform( program_id, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name );
                name[name_len] = 0;
                printf( "adding used unifom %s\n", name );
                auto normalized_name =StringReplace( std::string(name), "[0]", "");
                
                used_uniforms[normalized_name] = ShaderUniformDetails( num, type);
            }
        }
        else
        {
            GLint blen = 0;
            GLsizei slen = 0;
            glGetShaderiv(program_id, GL_INFO_LOG_LENGTH , &blen);
            if (blen > 1)
            {
                GLchar* link_log = new GLchar[blen];
                glGetProgramInfoLog(program_id, blen, &slen, link_log);
                errors = std::string( link_log );
                delete [] link_log;
            }
        }
        
        vertex_shader.Free();
        pixel_shader.Free();
    }    
}

GLuint OpenGL::ShaderProgram::GetProgramId(){
    return program_id;
}

std::map<std::string, OpenGL::ShaderUniformDetails> OpenGL::ShaderProgram::GetUsedUniforms(){
    return used_uniforms;
}

std::vector<OpenGL::ShaderAttributeDetails> OpenGL::ShaderProgram::GetInputAttributes(){
    return input_attributes;
}

void OpenGL::ShaderProgram::Enable( const unsigned int vertex_stride )
{
    glUseProgram( program_id );
    
    char* pointer_offset = nullptr;
    
    for( unsigned int i = 0; i < input_attributes.size(); i++ )
    {
        glEnableVertexAttribArray( i );
        auto attribute_details = input_attributes[i];
        
        int size = -1;
        if( "vec3" == attribute_details.type ){
            size = 3;
        }
        
        if( "vec4" == attribute_details.type ){
            size = 4;
        }

        glVertexAttribPointer( i, size, GL_FLOAT, GL_FALSE, vertex_stride, pointer_offset);
        pointer_offset += size * sizeof(float);
    }
}

GLint OpenGL::ShaderProgram::GetUniformLocation( const std::string& variable_name )
{
    return glGetUniformLocation( GetProgramId(), variable_name.c_str() );
}

bool OpenGL::ShaderProgram::SetInt( const std::string& variable_name, const int i ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniform1i( uniform_location, i );
        return true;
    }
    return false;
}

bool OpenGL::ShaderProgram::SetFloat( const std::string& variable_name, float flt ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniform1f( uniform_location, flt );
        return true;
    }
    return false;
}

bool OpenGL::ShaderProgram::SetFloat2( const std::string& variable_name, GeoFloat2 float2 ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniform2fv( uniform_location, 1, (float*) &float2 );
        return true;
    }
    return false;
}


bool OpenGL::ShaderProgram::SetFloat3( const std::string& variable_name, GeoFloat3 float3 ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniform3fv( uniform_location, 1, (float*) &float3 );
    }
    return false;
}

bool OpenGL::ShaderProgram::SetFloat3( const std::string& variable_name, GeoVector float3 ){
    return SetFloat3(variable_name, float3.ToGeoFloat3() );
}

bool OpenGL::ShaderProgram::SetFloat3( const std::string& variable_name, GeoFloat4 f ){
    return SetFloat3(variable_name, GeoFloat3( f.x, f.y, f.z ) );
}

bool OpenGL::ShaderProgram::SetFloat4( const std::string& variable_name, GeoFloat4 float4 ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniform4fv( uniform_location, 1, (float*) &float4 );
    }
    return false;
}

bool OpenGL::ShaderProgram::SetFloat4( const std::string& variable_name, GeoVector float4 ){
    return SetFloat4( variable_name, float4.ToGeoFloat4() );
}

bool OpenGL::ShaderProgram::SetMatrix( const std::string& variable_name, GeoMatrix& matrix ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glUniformMatrix4fv( uniform_location, 1, GL_FALSE, (float*) &matrix );
        return true;
    }
    return false;
}

bool OpenGL::ShaderProgram::SetTexture( const std::string& variable_name, Texture& texture, const GLenum texture_index ){
    auto uniform_location = GetUniformLocation( variable_name );
    if( -1 != uniform_location ){
        glActiveTexture( GL_TEXTURE0 + texture_index );
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, texture.GetOpenGLTextureId() );
        glActiveTexture( GL_TEXTURE0 );
        glUniform1i( uniform_location, texture_index );

        return true;
    }
    return false;
}



