#pragma once

#include "lights.h"
#include "drawable.h"


namespace SceneGraph{
    struct Node {
        Node();
        virtual ~Node();
        virtual bool Update( const float elapsed_seconds );
        virtual GeoMatrix GetUpdatedLocalTransform();
        std::vector<Node*> children;
        GeoMatrix cached_world_transform;
        GeoMatrix local_transform;
        bool cached_world_transform_set;
    protected:
    };
    
    struct Rotor : public Node {
        Rotor();
        virtual ~Rotor();
        GeoVector local_rotation_axis;
        float local_rotation_degrees;
        float local_rotation_speed;
        virtual bool Update( const float elapsed_seconds );
        virtual GeoMatrix GetUpdatedLocalTransform();
    };
    
    struct LightNode : public Node, IFullLightDetails {
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