#include "node.h"
#include "../../../shapes/quad.h"





SceneGraph::Node::Node(){
    local_transform = GeoMatrix::Identity();
    
    
}

SceneGraph::Node::~Node(){
    
}

GeoMatrix SceneGraph::Node::GetUpdatedLocalTransform(){
    return local_transform;
}


bool SceneGraph::Node::Update( const float elapsed_seconds ){
    
    return true;
}

SceneGraph::Rotor::~Rotor(){
    
}


bool SceneGraph::Rotor::Update( const float elapsed_seconds ){
    local_rotation_degrees += elapsed_seconds * local_rotation_speed;
    return true;
}

GeoMatrix SceneGraph::Rotor::GetUpdatedLocalTransform(){
    return local_transform * GeoQuaternion(local_rotation_axis, local_rotation_degrees).ToMatrix();
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
