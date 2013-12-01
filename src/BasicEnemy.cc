#include <random>
#include <chrono>

#include "BasicEnemy.hh"

using namespace std;

BasicEnemy::BasicEnemy(string filename, const Sprite &reference,
                       unsigned short width, unsigned short height,
                       int edge_coord) :
  Sprite(filename, 0, 0, width, height)
{
  size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed);

  uniform_int_distribution<int> rand_x(-edge_coord + width/2, edge_coord - width/2);
  this->x_ = rand_x(gen);

  //uniform_int_distribution<int> rand_y(50, 150);
  this->y_ = reference.y() + 50;
}

BasicEnemy::~BasicEnemy() {}
