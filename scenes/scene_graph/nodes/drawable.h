#pragma once

#include <vector>
#include <map>

#include "../../../shapes/shape.h"
#include "../../../ogl/buffers/vertex_buffers/vertex_buffer.h"
#include "../materials/material.h"



namespace SceneGraph{
    
    struct TextureDetails{
        std::string texture_name;
        GeoFloat2 scale;
        GeoFloat2 offset;
        TextureDetails();
        TextureDetails( std::string texture_name );
        TextureDetails( std::string texture_name, GeoFloat2 scale );
        TextureDetails( std::string texture_name, const float scale );
    };
    
    
    struct IDrawable {
        virtual ~IDrawable();
        OpenGL::VertexBuffer<Vertex> vertex_buffer;
        Material material;
        std::map< std::string, TextureDetails> textures;
        std::string shader_program;
    };
}