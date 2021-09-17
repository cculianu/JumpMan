/*!
 * \file MovingStar.cc
 * \brief File containing the MovingStar class source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 */
#include "MovingStar.h"

#include "Game.h"

#include <chrono>
#include <random>


MovingStar::MovingStar(short y, int edge_coord) : BasicStar(y, edge_coord)
{
    auto gen = Game::GetRandGen(-5, 5);

    dx = gen();
    dy = gen();

    this->filename_ = "moving_star";
}

MovingStar::~MovingStar() {}

void MovingStar::takeAction(double dt)
{
    BasicStar::takeAction(dt);
    x_ += dx * dt;
    y_ += dy * dt;
}
