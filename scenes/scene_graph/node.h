#include <vector>
#include <map>

#include "../../shapes/shape.h"
#include "../../geo/GeoMatrix.h"
#include "../../ogl/buffers/vertex_buffers/vertex_buffer.h"



namespace SceneGraph{
    //struct Texture{
    //};
    
    struct Node {
        Node();
        virtual ~Node();
        std::vector<Node*> children;
        GeoMatrix local_transform;
    };
    
    struct Geode : public Node {
        virtual ~Geode();
        OpenGL::VertexBuffer<Vertex> vertex_buffer;
        std::map< std::string, std::string> textures;
        std::string shader_program;
    };
}