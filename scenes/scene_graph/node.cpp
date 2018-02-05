#include "node.h"
#include "../../shapes/quad.h"

SceneGraph::ILightDetails::ILightDetails(){
    directional = false;
}

SceneGraph::IDrawable::~IDrawable(){
    
}

SceneGraph::Node::Node(){
    local_transform = GeoMatrix::Identity();
    
    
}

SceneGraph::Node::~Node(){
    
}

SceneGraph::LightNode::~LightNode(){
    
}

SceneGraph::Geode::~Geode(){
    
}

SceneGraph::LightGeode::~LightGeode(){
    
}

SceneGraph::Sprite::~Sprite(){
    
}

SceneGraph::BillboardSprite::~BillboardSprite(){
    
}

SceneGraph::Sprite::Sprite(){
    vertex_buffer = OpenGL::VertexBuffer<Vertex>( Quad::XYUnitQuad().ToVertices() );
}


