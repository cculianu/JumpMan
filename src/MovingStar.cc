/*!
 * \file MovingStar.cc
 * \brief File containing the MovingStar class source code
 *
 * \author Olle Kvarnström
 * \date
 */

#include <chrono>
#include <random>

#include "MovingStar.hh"

using namespace std;

MovingStar::MovingStar(short y, int edge_coord) :
  BasicStar(y, edge_coord),
  dx_(0),
  dy_(0)
{
  default_random_engine 
    gen(chrono::system_clock::now().time_since_epoch().count());
  uniform_int_distribution<int> rand(-5, 5);

  this->dx_ = rand(gen);
  this->dy_ = rand(gen);

  this->filename_ = "moving_star";
}

MovingStar::~MovingStar() {}

void MovingStar::takeAction()
{
  this->x_ += this->dx_;
  this->y_ += this->dy_;
}
