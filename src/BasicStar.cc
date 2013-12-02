#include <random>
#include <chrono>

#include "BasicStar.hh"

using namespace std;

BasicStar::BasicStar(short y, int edge_coord) :
  Sprite("star_yellow", 0, y + 50, 20, 20, 4)
{ 
  randomizeSpawn(edge_coord); 
}

BasicStar::~BasicStar() {}

void BasicStar::randomizeSpawn(int edge_coord)
{
  size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed);

  uniform_int_distribution<int> 
    rand_x(-edge_coord + this->width_/2, edge_coord - this->width_/2);
  this->x_ = rand_x(gen);
}
