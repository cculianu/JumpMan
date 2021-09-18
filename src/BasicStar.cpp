/*!
 * \file BasicStar.cpp
 * \brief File containing the BasicStar source code
 *
 * \author Olle Kvarnström
 * \date 2013
 * \copyright GNU Public License
 *
 * Heavily modified by Calin A. Culianu <calin.culianu@gmail.com>
 */
#include "BasicStar.h"

#include "Game.h"

#include <chrono>


BasicStar::BasicStar(short y, int edge_coord) : Sprite("basic_star", 0, y + 50, 20, 20, 4)
{
    this->randomizeSpawn(edge_coord);
}

BasicStar::~BasicStar() {}

void BasicStar::randomizeSpawn(int edge_coord)
{
    this->x_ = Game::GetRand32(-edge_coord + this->width_ / 2, edge_coord - this->width_ / 2);
}
