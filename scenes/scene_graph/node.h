#include <vector>
#include <map>

#include "../../shapes/shape.h"
#include "../../geo/GeoMatrix.h"
#include "../../ogl/buffers/vertex_buffers/vertex_buffer.h"



namespace SceneGraph{
   
    
    struct IDrawable {
        OpenGL::VertexBuffer<Vertex> vertex_buffer;
        std::map< std::string, std::string> textures;
        std::string shader_program;
    };
    
    
    
    
    
    struct Node {
        Node();
        virtual ~Node();
        std::vector<Node*> children;
        GeoMatrix local_transform;
    };
    
    struct LightNode{
        bool directional;
        GeoVector direction;
    };
    
    
    struct Geode : public Node, IDrawable {
        virtual ~Geode();
    };
}