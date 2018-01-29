#include "node.h"

SceneGraph::Node::Node(){
    local_transform = GeoMatrix::Identity();
    
    
}

SceneGraph::Node::~Node(){
    
}

SceneGraph::Geode::~Geode(){
    
}


