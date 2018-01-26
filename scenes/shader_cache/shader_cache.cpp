#include "shader_cache.h"
#include "../../io/file_io.h"
#include <iostream>

std::string ShaderCache::vertex_shader_path;
std::string ShaderCache::pixel_shader_path;


void ShaderCache::ConfigureShaderPaths( const std::string& v_path, const std::string& p_path ){
    vertex_shader_path = v_path;
    pixel_shader_path = p_path;
}

ShaderCache::ShaderCache(){
    active_shader_program = nullptr;
}

std::string ShaderCache::ResolveShaderFilepath( const std::string& shader_path, const std::string& name, const std::string& ext ){
    return File::PathJoin( shader_path, name ) + "." + ext;
}

bool ShaderCache::IsNameRegistered( const std::string& name ){
    return shader_programs.find(name) != shader_programs.end();
}

bool ShaderCache::RegisterShaderProgram( std::string name ){
    return RegisterShaderProgram( name, name, name );
}

bool ShaderCache::RegisterShaderProgram( std::string name, std::string vertex_shader_name, std::string pixel_shader_name ){
    
    auto full_vertex_shader_path = ResolveShaderFilepath(vertex_shader_path, vertex_shader_name, "vert");
    auto full_pixel_shader_path = ResolveShaderFilepath(pixel_shader_path, pixel_shader_name, "frag");
    
    std::cout << "full_vertex_shader_path: " << full_vertex_shader_path << "\n";
    std::cout << "full_pixel_shader_path: " << full_pixel_shader_path << "\n";

    
    auto vertex_shader_source = File::ReadAllTextWithIncludePreprocessing(full_vertex_shader_path);
    auto pixel_shader_source = File::ReadAllTextWithIncludePreprocessing(full_pixel_shader_path);
    
    auto program = new OpenGL::ShaderProgram( vertex_shader_source, pixel_shader_source );
    
    if( program->IsValid() ){
        shader_programs[name] = program;
        return true;
    }
    
    return false;
}

void ShaderCache::UnregisterShaderProgram( std::string name ){
    if( !IsNameRegistered(name) ){
        auto program = shader_programs[name];
        delete program;
        shader_programs.erase( name );
    }
        
}

bool ShaderCache::ActivateShaderProgram( std::string name, const unsigned int vertex_stride ){
    if( !IsNameRegistered(name) )
        return false;
    active_shader_program = shader_programs[name];
    active_shader_program->Enable(vertex_stride);
    return true;
}

bool ShaderCache::SetInt( const std::string& variable_name, const int i ){
    if( active_shader_program )
        return active_shader_program->SetInt( variable_name, i );
    return false;
}

bool ShaderCache::SetFloat( const std::string& variable_name, float flt ){
    if( active_shader_program )
        return active_shader_program->SetFloat( variable_name, flt );
    return false;
}

bool ShaderCache::SetFloat3( const std::string& variable_name, GeoVector float3 ){
    if( active_shader_program )
        return active_shader_program->SetFloat3( variable_name, float3 );
    return false;
}

bool ShaderCache::SetFloat3( const std::string& variable_name, GeoFloat3 float3 ){
    if( active_shader_program )
        return active_shader_program->SetFloat3( variable_name, float3 );
    return false;
}

bool ShaderCache::SetFloat3( const std::string& variable_name, GeoFloat4 f ){
    if( active_shader_program )
        return active_shader_program->SetFloat3( variable_name, f );
    return false;
}

bool ShaderCache::SetFloat4( const std::string& variable_name, GeoVector float4 ){
    if( active_shader_program )
        return active_shader_program->SetFloat4( variable_name, float4 );
    return false;
}

bool ShaderCache::SetFloat4( const std::string& variable_name, GeoFloat4 float4 ){
    if( active_shader_program )
        return active_shader_program->SetFloat4( variable_name, float4 );
    return false;
}

bool ShaderCache::SetMatrix( const std::string& variable_name, GeoMatrix matrix ){
    if( active_shader_program )
        return active_shader_program->SetMatrix( variable_name, matrix );
    return false;
}