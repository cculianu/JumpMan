#include <random>
#include <chrono>

#include "BasicEnemy.hh"

using namespace std;

BasicEnemy::BasicEnemy(string filename, const Sprite &reference,
                       unsigned short width, unsigned short height) :
  Sprite(filename, 0, 0, width, height)
{
  size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed);
  uniform_int_distribution<int> rand(100, 200);

  /* Set X to somewhere in the range x_ - rand() - x_ + rand() */
  this->x_ = reference.x() + rand(gen) % 2 == 0 ? rand(gen) : -rand(gen);

  /* Set Y to y_ + rand() */
  this->y_ = reference.y() + rand(gen);
}

BasicEnemy::~BasicEnemy() {}
