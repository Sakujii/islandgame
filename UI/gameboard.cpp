#include "gameboard.hh"
#include "mainwindow.hh"
#include "actor.hh"
#include "transport.hh"
#include "boardhex.hh"

#include <iostream>
#include <QtWidgets/QMainWindow>


namespace Student{

GameBoard::GameBoard() = default;

GameBoard::~GameBoard() = default;


int GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{
    return tileCoord.x;
}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{
    std::cout << tileCoord.x << std::endl;
    return true;
}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{
    std::cout << hexCoord.x << std::endl;
    return std::shared_ptr<Common::Hex>();
}

void GameBoard::addPawn(int playerId, int pawnId)
{
    std::cout << playerId << pawnId << std::endl;
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    std::cout << playerId << pawnId << coord.x << std::endl;
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    std::cout << pawnId << pawnCoord.x << std::endl;
}

void GameBoard::removePawn(int pawnId)
{
    std::cout << pawnId << std::endl;
}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{
    int id = actor->getId();
    std::cout << id << actorCoord.x << std::endl;
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    std::cout << actorId << actorCoord.x << std::endl;
}

void GameBoard::removeActor(int actorId)
{
    std::cout << actorId << std::endl;
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate coord = newHex->getCoordinates();
    // TODO: Koordinaattimuunnos ruudulle
    Student::MainWindow* win = Student::MainWindow::getInstance();
    win->drawHex(coord);
    hexMap_.insert(std::make_pair(coord, newHex));

}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{
    std::cout << transport->getId() << coord.x << coord.y << coord.z << std::endl;
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    std::cout << id << coord.x << coord.y << coord.z << std::endl;
}

void GameBoard::removeTransport(int id)
{
    std::cout << id << std::endl;
}

}
