#pragma once

struct GeoFloat2
{
	GeoFloat2();
	GeoFloat2( const float x, const float y );
	float x,y;
};

struct GeoFloat3
{
	GeoFloat3();
	GeoFloat3( const float x, const float y, const float z );
    GeoFloat3( const int x, const int y, const int z );

	float x,y,z;
};

struct GeoFloat4
{
	GeoFloat4();
	GeoFloat4( const float x, const float y, const float z, const float w );
	float x,y,z, w;
};

