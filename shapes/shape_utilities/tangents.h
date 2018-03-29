#include "../shape.h"


//https://github.com/mlimper/tgen/blob/master/src/tgen.cpp







std::pair< std::vector<GeoVector>, std::vector<GeoVector> > CalculateMirroredSphereTangentArray(
                                                                                  std::vector<GeoFloat3> vertex,
                                                                                  std::vector<GeoVector> normal,
                                                                                  std::vector<GeoFloat2> texcoord,
                                                                                  std::vector<unsigned int> indices
                                                                                  );


std::pair< std::vector<GeoVector>, std::vector<GeoVector> > CalculateTangentArray(
                                                                                  std::vector<GeoFloat3> vertex,
                                                                                  std::vector<GeoVector> normal,
                                                                                  std::vector<GeoFloat2> texcoord,
                                                                                  std::vector<unsigned int> indices
                                                                                 );



std::vector<Vertex> CalculateTangentArray( const std::vector<unsigned int>& indices, const std::vector<Vertex>& vertices );



void CalculateTangentArray( std::vector<Vertex>& vertices );
