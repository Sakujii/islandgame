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
    bool a = false;
    for(auto s:_playerPointVector)
        if(s.first==playerid)
        {
            s.second+= points;
            a = true;
        }
    if(a==false)
    {
        _playerPointVector.push_back(std::make_pair(playerid,points));
    }
}




}
