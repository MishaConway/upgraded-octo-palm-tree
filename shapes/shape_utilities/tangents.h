#include "../shape.h"


//https://github.com/mlimper/tgen/blob/master/src/tgen.cpp


std::pair< std::vector<GeoVector>, std::vector<GeoVector> > CalculateTangentArray(
                                                                                  std::vector<GeoFloat3> vertex,
                                                                                  std::vector<GeoVector> normal,
                                                                                  std::vector<GeoFloat2> texcoord,
                                                                                  std::vector<unsigned int> indices
                                                                                  );

void CalculateTangentArray( std::vector<Vertex>& vertices );
