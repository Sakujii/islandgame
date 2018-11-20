#include "gamestate.hh"

namespace Student{

GameState::GameState ()
{
    _playerInTurn = 0;
    _pawnPerPlayer= 0;
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

void GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    _gamePhaseId = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    _playerInTurn=nextPlayer;
}


}
