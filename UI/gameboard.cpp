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
    // If hex does not exist, return -1
    if (hexMap_.find(tileCoord) == hexMap_.end()){
        return -1;
    }
    // Otherwise return Pawn amount
    else {
        auto it = hexMap_.find(tileCoord);
        std::shared_ptr<Common::Hex> hex = it->second;
        int result = hex->getPawnAmount();

        return result;
    }
}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{
    std::shared_ptr<Common::Hex> hex = getHex(tileCoord);

    if (hex != nullptr){
        auto it = hexMap_.find(tileCoord);
        std::shared_ptr<Common::Hex> hex = it->second;
        bool answer = hex->isWaterTile();
        return answer;
    }
    else{
        return false;
    }

}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{
    auto it = hexMap_.find(hexCoord);
    if (it == hexMap_.end()){
        return nullptr;
    }
    else{
        std::shared_ptr<Common::Hex> hex = it->second;
        return hex;
    }
}

void GameBoard::addPawn(int playerId, int pawnId)
{
    Common::Pawn *newPawn = new Common::Pawn();
    newPawn->setId(pawnId, playerId);
    auto it = pawnMap_.find(pawnId);
    if (it == pawnMap_.end()){
        pawnMap_.insert(std::make_pair(pawnId, newPawn));
    }
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Pawn> newPawn = std::make_shared<Common::Pawn>();
    newPawn->setId(pawnId, playerId);
    newPawn->setCoordinates(coord);

    auto it = pawnMap_.find(pawnId);
    if (it == pawnMap_.end()){
        std::shared_ptr<Common::Hex> hex = getHex(coord);
        hex->addPawn(newPawn);
        pawnMap_.insert(std::make_pair(pawnId, newPawn));
    }
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    // If Hex exists, move Pawn
    if (hexMap_.find(pawnCoord) != hexMap_.end()){
        auto it = pawnMap_.find(pawnId);
        if (it != pawnMap_.end()){

            // Remove Pawn from old Hex
            std::shared_ptr<Common::Pawn> pawnPtr = it->second;
            Common::CubeCoordinate oldCoord = pawnPtr->getCoordinates();
            std::shared_ptr<Common::Hex> oldHex = getHex(oldCoord);
            oldHex->removePawn(pawnPtr);

            // Add pawn to new Hex and update Pawn coordinates
            std::shared_ptr<Common::Hex> newHex = getHex(pawnCoord);
            newHex->addPawn(pawnPtr);
            pawnPtr->setCoordinates(pawnCoord);
        }
    }
}

void GameBoard::removePawn(int pawnId)
{
    auto it = pawnMap_.find(pawnId);
    if (it != pawnMap_.end()){
        std::shared_ptr<Common::Pawn> pawnPtr = it->second;
        Common::CubeCoordinate oldCoord = pawnPtr->getCoordinates();
        std::shared_ptr<Common::Hex> oldHex = getHex(oldCoord);
        oldHex->removePawn(pawnPtr);

        pawnMap_.erase(it);
    }
}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{

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
    //Student::MainWindow* win = Student::MainWindow::getInstance();
    //win->drawHex(coord);

    auto it = hexMap_.find(coord);
    if (it == hexMap_.end() ){
        hexMap_.insert(std::make_pair(coord, newHex));
    }
    else{
        it->second = newHex;
    }

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
