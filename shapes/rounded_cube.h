#pragma once

#include "cube.h"



class RoundedCube : public Cube
{
public:
    RoundedCube(const int meshResolution, const double cornerStrength );
    
    virtual ~RoundedCube();
    static RoundedCube UnitRoundedCube();
    static RoundedCube UnitRoundedCube(const int meshResolution, const double cornerStrength );
    
protected:
    void GenerateQuads(  const int meshResolution, const double cornerStrength );
};