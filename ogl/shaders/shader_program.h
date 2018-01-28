#include "shader.h"
#include "../textures/texture.h"
#include "../../geo/GeoVector.h"
#include "../../geo/GeoMatrix.h"
#include "../../interfaces/IValidatable.h"
#include "../../interfaces/IHasErrors.h"
#include <vector>


namespace OpenGL{
    struct ShaderAttributeDetails{
        ShaderAttributeDetails( std::string name, std::string type);
        std::string name;
        std::string type;
    };
    
    struct ShaderUniformDetails{
        ShaderUniformDetails();
        ShaderUniformDetails( GLint size, GLenum );
        GLint size;
        GLenum type;
    };
    
    
    
    class ShaderProgram : public IValidatable, public IHasErrors
    {
    public:
        ShaderProgram( const std::string& vertex_shader_source, const std::string& pixel_shader_source );
        GLuint GetProgramId();
        std::map<std::string, ShaderUniformDetails> GetUsedUniforms();
        std::vector<ShaderAttributeDetails> GetInputAttributes();
        bool UsingUniform( const std::string& uniform_name );
        void Enable( const unsigned int vertex_stride );
    public:
        bool SetInt( const std::string& variable_name, const int i );
        bool SetFloat( const std::string& variable_name, float flt );

        bool SetFloat3( const std::string& variable_name, GeoVector float3 );
        bool SetFloat3( const std::string& variable_name, GeoFloat3 float3 );
        bool SetFloat3( const std::string& variable_name, GeoFloat4 float_array );
        
        bool SetFloat4( const std::string& variable_name, GeoVector float4 );
        bool SetFloat4( const std::string& variable_name, GeoFloat4 float4 );

        bool SetMatrix( const std::string& variable_name, GeoMatrix& matrix );
       
        bool SetTexture( const std::string& variable_name, Texture& texture, const GLenum texture_index );
    protected:
        GLint GetUniformLocation( const std::string& variable_name );
    protected:
        GLuint program_id;
        std::map<std::string, ShaderUniformDetails> used_uniforms;
        std::vector<ShaderAttributeDetails> input_attributes;
    };
}

