#pragma once
#include "GeoVector.h"
#include "GeoMatrix.h"


/* Four component vector */
class GeoQuaternion : public IStringable
{
public:
	GeoQuaternion();
	GeoQuaternion( const GeoVector& rotation_axis, const float degrees );
	GeoQuaternion operator * ( const GeoQuaternion& ) const;
	GeoMatrix ToMatrix();
    GeoVector GetAxis();
    float     GetAngle();
    virtual std::string ToString();
private:
	float w, x, y, z;
};
 
