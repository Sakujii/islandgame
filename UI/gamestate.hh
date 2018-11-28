#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"
#include <vector>

namespace Student{


class GameState : public Common::IGameState
{
public:
    /**
     * @brief Constructor, exists solely for documentation.
     */
    GameState();

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
     */
    ~GameState();

    /**
     * @brief currentGamePhase tells the phase of the game.
     * @return The phase of the game.
     * @post Exception quarantee: nothrow
     */
    Common::GamePhase currentGamePhase() const;

    /**
     * @brief currentPlayer tells the player in turn.
     * @return The identifier of the player in turn.
     * @post Excetion quarantee: nothrow
     */
    int currentPlayer() const;

    /**
     * @brief changeGamePhase sets the phase of the game.
     * @param newPhase The next phase of the game.
     * @post Game phase changed. Exception quarantee: basic
     */
    void changeGamePhase(Common::GamePhase nextPhase);

    /**
     * @brief changePlayerTurn sets the player in turn.
     * @param nextPlayer The identifier of the next player in turn.
     * @post Turn changed to player nextPlayer. Exception quarantee: basic
     */
    void changePlayerTurn(int nextPlayer);

    /**
     * @brief addPointsToPlayer adds points to a given player.
     * @param playerid The id of the player.
     * @param points The number of points to add.
     * @post Points are added to player. Exception quarantee: basic
     */
    void addPointsToPlayer(int playerid, int points);

    int getPawnPerPlayer();
    void deletePawn(int pawn);
    int playerPawnsLeft(int playerid);


private:
    Common::GamePhase _gamePhaseId;
    int _playerInTurn;
    int _pawnPerPlayer;
    std::vector<std::pair<int,int>> _playerPointVector;
    std::vector<std::pair<int,int>> _playerPawnVector;

};
}

#endif // GAMESTATE_HH
