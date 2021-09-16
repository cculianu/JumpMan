/*!
 * \file BasicStar.cc
 * \brief File containing the BasicStar source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "BasicStar.h"

#include <random>
#include <chrono>


using namespace std;

BasicStar::BasicStar(short y, int edge_coord) :
  Sprite("basic_star", 0, y + 50, 20, 20, 4)
{ 
  this->randomizeSpawn(edge_coord); 
}

BasicStar::~BasicStar() {}

void BasicStar::randomizeSpawn(int edge_coord)
{
  default_random_engine 
    gen(std::chrono::system_clock::now().time_since_epoch().count());

  uniform_int_distribution<int> 
    rand_x(-edge_coord + this->width_/2, 
            edge_coord - this->width_/2);

  this->x_ = rand_x(gen);
}

void BasicStar::takeAction() {}
