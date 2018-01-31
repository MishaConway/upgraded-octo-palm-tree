#include "node.h"
#include "../../shapes/quad.h"

SceneGraph::Node::Node(){
    local_transform = GeoMatrix::Identity();
    
    
}

SceneGraph::Node::~Node(){
    
}

SceneGraph::Geode::~Geode(){
    
}

SceneGraph::Sprite::~Sprite(){
    
}

SceneGraph::BillboardSprite::~BillboardSprite(){
    
}

SceneGraph::Sprite::Sprite(){
    vertex_buffer = OpenGL::VertexBuffer<Vertex>( Quad::XYUnitQuad().ToVertices() );
}


