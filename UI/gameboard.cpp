#include "gameboard.hh"
#include "actor.hh"
#include "transport.hh"

#include <iostream>


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
    std::shared_ptr<Common::Pawn> newPawn = std::make_shared<Common::Pawn>();
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
    int actorId = actor->getId();

    auto it = actorMap_.find(actorId);
    if (it == actorMap_.end()){
        std::shared_ptr<Common::Hex> hex = getHex(actorCoord);
        actor->addHex(hex);
        actorMap_.insert(std::make_pair(actorId, actor));
    }
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    // If Hex exists, move Actor
    if (hexMap_.find(actorCoord) != hexMap_.end()){
        auto it = actorMap_.find(actorId);
        if (it != actorMap_.end()){
            std::shared_ptr<Common::Actor> actor = it->second;
            std::shared_ptr<Common::Hex> newHex = getHex(actorCoord);
            actor->move(newHex);
        }
    }
}

void GameBoard::removeActor(int actorId)
{
    auto it = actorMap_.find(actorId);
    if (it != actorMap_.end()){
        std::shared_ptr<Common::Actor> actor = it->second;
        std::shared_ptr<Common::Hex> hex = actor->getHex();
        hex->removeActor(actor);

        actorMap_.erase(it);
    }
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate coord = newHex->getCoordinates();

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
    int transportId = transport->getId();

    auto it = transportMap_.find(transportId);
    if (it == transportMap_.end()){
        std::shared_ptr<Common::Hex> hex = getHex(coord);
        transport->addHex(hex);
        transportMap_.insert(std::make_pair(transportId, transport));

    }
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    // If hex exists, move transport and pawns
    if (hexMap_.find(coord) != hexMap_.end()){
        auto it = transportMap_.find(id);
        if (it != transportMap_.end()){
            std::shared_ptr<Common::Transport> transport = it->second;
            std::shared_ptr<Common::Hex> newHex = getHex(coord);
            transport->move(newHex);
        }

    }
}

void GameBoard::removeTransport(int id)
{
    auto it = transportMap_.find(id);
    if (it != transportMap_.end()){
        std::shared_ptr<Common::Transport> transport = it->second;
        std::shared_ptr<Common::Hex> hex = transport->getHex();
        hex->removeTransport(transport);

        transportMap_.erase(it);
    }
}

std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > GameBoard::getHexMap()
{
    return hexMap_;
}

std::unordered_map<int, std::shared_ptr<Common::Pawn> > GameBoard::getPawnMap()
{
    return pawnMap_;
}

}
