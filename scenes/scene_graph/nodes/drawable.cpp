#include "drawable.h"

SceneGraph::IDrawable::~IDrawable(){
    
}

SceneGraph::TextureDetails::TextureDetails(){
}

SceneGraph::TextureDetails::TextureDetails( std::string texture_name ){
    this->texture_name = texture_name;
    scale = GeoFloat2(1,1);
}

SceneGraph::TextureDetails::TextureDetails( std::string texture_name, GeoFloat2 scale ){
    this->texture_name = texture_name;
    this->scale = scale;
}