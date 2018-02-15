#include "../shape.h"




std::vector<Vertex> CalculateDebugNormalLines( std::vector<Vertex> vertices, const float length );
std::vector<Vertex> CalculateDebugTangentLines( std::vector<Vertex> vertices, const float length );
std::vector<Vertex> CalculateDebugBiTangentLines( std::vector<Vertex> vertices, const float length );



std::pair< GeoFloat2, GeoFloat2 > FindMinMaxUvs( std::vector<Vertex> vertices );


