#include <vector>
#include <map>

#include "../../shapes/shape.h"
#include "../../geo/GeoMatrix.h"
#include "../../ogl/buffers/vertex_buffers/vertex_buffer.h"



namespace SceneGraph{
   
    
    struct IDrawable {
        virtual ~IDrawable();
        OpenGL::VertexBuffer<Vertex> vertex_buffer;
        std::map< std::string, std::string> textures;
        std::string shader_program;
    };
    
    struct ILightDetails{
        bool directional;
        GeoFloat3 attenuation;
        //vec3 Direction;
        GeoFloat3 color;
        ILightDetails();
    };
    
 
    

    
    //struct ITranslating
    
    struct Node {
        Node();
        virtual ~Node();
        virtual bool Update( const float elapsed_seconds );
        virtual GeoMatrix GetUpdatedLocalTransform();
        std::vector<Node*> children;
        GeoMatrix cached_world_transform;
        GeoMatrix local_transform;
    protected:
    };
    
    struct Rotor : public Node {
        virtual ~Rotor();
        GeoVector local_rotation_axis;
        float local_rotation_degrees;
        float local_rotation_speed;
        virtual bool Update( const float elapsed_seconds );
        virtual GeoMatrix GetUpdatedLocalTransform();
    };
    
    struct LightNode : public Node, ILightDetails {
        virtual ~LightNode();
    };
    
    struct Geode : public Node, IDrawable {
        virtual ~Geode();
    };
    
    
    struct LightGeode : public LightNode, IDrawable {
        virtual ~LightGeode();
    };
    
    
    struct Sprite : public Geode {
        virtual ~Sprite();
        Sprite();
    };
    
    struct BillboardSprite : public Sprite {
        virtual ~BillboardSprite();
    };
    
    
}