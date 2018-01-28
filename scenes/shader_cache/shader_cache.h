#pragma once


#include <map>
#include "../../ogl/shaders/shader_program.h"
#include "../../ogl/textures/texture.h"




class ShaderCache{
public:
    ShaderCache();
    
    static void ConfigureShaderPaths( const std::string& vertex_shader_path, const std::string& pixel_shader_path );
    
    bool RegisterShaderProgram( std::string name );
    bool RegisterShaderProgram( std::string name, std::string vertex_shader_name, std::string pixel_shader_name );
    void UnregisterShaderProgram( std::string name );
    
    bool IsNameRegistered( const std::string& name );
    
    
    bool ActivateShaderProgram( std::string name, const unsigned int vertex_stride );
    
    bool SetInt( const std::string& variable_name, const int i );
    bool SetFloat( const std::string& variable_name, float flt );
    
    bool SetFloat3( const std::string& variable_name, GeoVector float3 );
    bool SetFloat3( const std::string& variable_name, GeoFloat3 float3 );
    bool SetFloat3( const std::string& variable_name, GeoFloat4 float_array );
    
    bool SetFloat4( const std::string& variable_name, GeoVector float4 );
    bool SetFloat4( const std::string& variable_name, GeoFloat4 float4 );
    
    bool SetMatrix( const std::string& variable_name, GeoMatrix matrix );
    
    bool SetTexture( const std::string& variable_name, OpenGL::Texture& texture, const GLenum texture_index );

    
protected:
    
    std::string ResolveShaderFilepath( const std::string& shader_path, const std::string& name, const std::string& ext );
    
    
    
protected:
    OpenGL::ShaderProgram* active_shader_program;
    std::map< std::string, OpenGL::ShaderProgram*> shader_programs;
    
    static std::string vertex_shader_path;
    static std::string pixel_shader_path;
};