#include "gamestate.hh"
#include <iostream>
#include <algorithm>

namespace Student{

GameState::GameState ()
{
    playerInTurn_ = 1;
    gamePhaseId_ = Common::GamePhase::MOVEMENT;
    spinAnimal_ = "nada";
    spinMoveCount_ = "0";

}

int GameState::getPlayerPoints(int playerid)
{
    std::pair<int,int> pointpair = playerPointVector_.at(playerid-1);
    int points = pointpair.second;
    return points;
}

Common::GamePhase GameState::currentGamePhase() const
{
    return gamePhaseId_;
}

int GameState::currentPlayer() const
{
    return playerInTurn_;
}

void GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    gamePhaseId_ = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    playerInTurn_=nextPlayer;
}

void GameState::addPointsToPlayer(int playerid, int points)
{
    for(auto s : playerPointVector_){
        if(s.first==playerid)
        {
            playerPointVector_.at(playerid-1) = std::make_pair(s.first,s.second+points);
        }
    }
}

void GameState::initPoints(int playercount)
{
    playerPointVector_.clear();
    for(int playerid=1; playerid<playercount+1; playerid++){
        playerPointVector_.push_back(std::make_pair(playerid,0));
    }
}

std::vector<std::string> GameState::getPointsVectorForPlot()
{
    std::vector<std::string> plotvector;
    for(auto s : playerPointVector_)
    {
        std::string row = "Player: " + std::to_string(s.first) + "   Points: " + std::to_string(s.second);
        plotvector.push_back(row);
    }
    return plotvector;
}

std::string GameState::currentGamePhaseString()
{
    std::string string;
    if(gamePhaseId_==Common::GamePhase::MOVEMENT)
    {
        string = "Movement";
    }
    else if(gamePhaseId_==Common::GamePhase::SINKING)
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
    spinAnimal_ = spinresult.first;
    spinMoveCount_ = spinresult.second;
}

std::string GameState::getSpinMovecount()
{
    return spinMoveCount_;
}

std::string GameState::getSpinAnimal()
{
    return spinAnimal_;
}

std::string GameState::getWinner()
{
    std::string winnerId = "";
    int maxpoints = 0;
    std::string winmessage = "";
    for(auto s : playerPointVector_)
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
    return spinsLeft_;
}

void GameState::setSpinsLeft(int spincount)
{
    spinsLeft_ = spincount;
}



}
