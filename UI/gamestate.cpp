#include "gamestate.hh"
#include <iostream>
#include <algorithm>

namespace Student{

GameState::GameState ()
{
    _playerInTurn = 1;
    _pawnPerPlayer= 5;
    _gamePhaseId = Common::GamePhase::MOVEMENT;
    _spinAnimal = "nada";
    _spinMoveCount = "0";

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

std::string GameState::currentGamePhaseString()
{
    std::string string;
    if(_gamePhaseId==Common::GamePhase::MOVEMENT)
    {
        string = "Movement";
    }
    else if(_gamePhaseId==Common::GamePhase::SINKING)
    {
        string = "Sinking";
    }
    else
    {
        string = "Spinning";
    }
    return string;
}

void GameState::setSpinResult(std::pair<std::string, std::string> spinresult)
{
    _spinAnimal = spinresult.first;
    _spinMoveCount = spinresult.second;
}

std::string GameState::getSpinMovecount()
{
    return _spinMoveCount;
}

std::string GameState::getSpinAnimal()
{
    return _spinAnimal;
}

std::string GameState::getWinner()
{
    std::string winnerId = "";
    int maxpoints = 0;
    std::string winmessage = "";
    for(auto s : _playerPointVector)
    {
        if(s.second > maxpoints)
        {
            winnerId = std::to_string(s.first);
            maxpoints = s.second;
        }
    }
    if(winnerId == "")
    {
        winmessage = "Game ended in a tie";
    }
    else
    {
        winmessage = "Player " + winnerId + " wins!";
    }
    return winmessage;
}

int GameState::getSpinsLeft()
{
    return _spinsLeft;
}

void GameState::setSpinsLeft(int spincount)
{
    _spinsLeft = spincount;
}



}
