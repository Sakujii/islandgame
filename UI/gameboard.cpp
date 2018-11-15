#include "gameboard.hh"
#include "mainwindow.hh"
#include <iostream>


namespace Student{

GameBoard::GameBoard(){}

GameBoard::~GameBoard(){}



int GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{

}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{

}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{

}

void GameBoard::addPawn(int playerId, int pawnId)
{

}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{

}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{

}

void GameBoard::removePawn(int pawnId)
{

}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{

}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{

}

void GameBoard::removeActor(int actorId)
{

}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{

}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{

}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{

}

void GameBoard::removeTransport(int id)
{

}

}
