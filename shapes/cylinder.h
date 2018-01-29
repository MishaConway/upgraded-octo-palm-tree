#pragma once

#include "shape.h"
#include "cube.h"

class Cylinder : public Cube
{
public:
    virtual ~Cylinder();
    Cylinder( const float width, const float height, const float length );
};