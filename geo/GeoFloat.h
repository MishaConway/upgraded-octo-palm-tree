#pragma once

struct GeoFloat3;

struct GeoFloat2
{
	GeoFloat2();
	GeoFloat2( const float x, const float y );
    GeoFloat2( const GeoFloat3& f );
	float x,y;
};

struct GeoFloat3
{
	GeoFloat3();
	GeoFloat3( const float x, const float y, const float z );
    GeoFloat3( const int x, const int y, const int z );
    GeoFloat3( const GeoFloat2& f );

	float x,y,z;
};

struct GeoFloat4
{
	GeoFloat4();
	GeoFloat4( const float x, const float y, const float z, const float w );
	float x,y,z, w;
};

