#include "gamestate.hh"
#include <iostream>
#include <algorithm>

namespace Student{

GameState::GameState ()
{
    _playerInTurn = 1;
    _pawnPerPlayer= 5;
    _gamePhaseId = Common::GamePhase::MOVEMENT;

}

GameState::~GameState()
{

}

Common::GamePhase GameState::currentGamePhase() const
{
    return _gamePhaseId;
}

int GameState::currentPlayer() const
{
    return _playerInTurn;
}

int GameState::getPawnPerPlayer()
{
    return _pawnPerPlayer;
}

void GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    _gamePhaseId = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    _playerInTurn=nextPlayer;
}

void GameState::addPointsToPlayer(int playerid, int points)
{
    for(auto s:_playerPointVector)
        if(s.first==playerid)
        {
            _playerPointVector.at(playerid-1) = std::make_pair(s.first,s.second+points);
        }
}

void GameState::initPoints(int playercount)
{
    for(int playerid=1; playerid<playercount+1; playerid++)
        _playerPointVector.push_back(std::make_pair(playerid,0));
}

std::vector<std::string> GameState::getPointsVectorForPlot()
{
    std::vector<std::string> plotvector;
    for(auto s:_playerPointVector)
    {
        std::string row = "Player: " + std::to_string(s.first) + "   Points: " + std::to_string(s.second);
        plotvector.push_back(row);
    }
    return plotvector;
}




}
