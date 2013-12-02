#include <random>
#include <chrono>

#include "BasicStar.hh"

using namespace std;

BasicStar::BasicStar(string filename, 
                     const Sprite &reference,
                     unsigned short width, 
                     unsigned short height,
                     int edge_coord, 
                     int rotations) :
  Sprite(filename, 0, 0, width, height, rotations)
{ randomizeSpawn(reference, edge_coord); }

BasicStar::~BasicStar() {}

void BasicStar::randomizeSpawn(const Sprite &reference, int edge_coord)
{
  size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed);

  uniform_int_distribution<int> rand_x(-edge_coord + width_/2, edge_coord - width_/2);
  this->x_ = rand_x(gen);

  //uniform_int_distribution<int> rand_y(50, 150);
  this->y_ = reference.y() + 50;
}
