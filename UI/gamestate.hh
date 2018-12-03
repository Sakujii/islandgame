#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"
#include <vector>
#include <iostream>

/**
 * @file
 * @brief Implements class gamestate
 */

namespace Student{

/**
 * @brief Offers a gamestate class to keep track
 * of the state of the game
 */
class GameState : public Common::IGameState
{
public:
    /**
     * @brief Constructor
     * @post Private variables are initialized
     */
    explicit GameState();

    /**
     * @brief Default destructor
     */
    ~GameState() = default;

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

    /**
     * @brief initPoints initializes the points of players
     * @param playerCount Number of the players
     * @post playerPointVector is initialized
     * @post Exception guarantee: no throw
     */
    void initPoints(int playerCount);

    /**
     * @brief getPointsVectorForPlot gets the point vector
     * @return vector of strings containing player points
     * @post Exception guarantee: no throw
     */
    std::vector<std::string> getPointsVectorForPlot();

    /**
     * @brief currentGamePhaseString gets the current gamephase
     * @return string containing current gamephase
     * @post Exception guarantee: no throw
     */
    std::string currentGamePhaseString();

    /**
     * @brief setSpinResult sets the spin result
     * @param spinResult Pair of strings containing animal and amount of moves
     * @post Sets the spinResult in private variables
     * @post Exception guarantee: no throw
     */
    void setSpinResult(std::pair<std::string, std::string> spinResult);

    /**
     * @brief getSpinMoveCount gets spin moves left
     * @return string containing the amount of spinner moves
     * @post Exception guarantee: no throw
     */
    std::string getSpinMovecount();

    /**
     * @brief getSpinAnimal gets the spin animal
     * @return string containing the animal moving with spinner
     * @post Exception guarantee: no throw
     */
    std::string getSpinAnimal();

    /**
     * @brief getWinner gets the winner of the game
     * @return string of player who won the game
     * @post Exception guarantee: no throw
     */
    std::string getWinner();

    /**
     * @brief getSpinsLeft gets the amount of spins left
     * @return Number of spins left for current phase
     * @post Exception guarantee: no throw
     */
    int getSpinsLeft();

    /**
     * @brief setSpinsLeft sets the amount of spins left
     * @param Number of spins left for current phase
     * @post Exception guarantee: no throw
     */
    void setSpinsLeft(int spincount);

    /**
     * @brief getPlayerPoints gets the points of the player
     * @param playerId Id of the player
     * @return Number of points of the player
     * @post Exception guarantee: no throw
     */
    int getPlayerPoints(int playerId);


private:
    //! Current gamephase
    Common::GamePhase gamePhaseId_;

    //! Player in turn
    int playerInTurn_;

    //! Vector containing player id and points of the player
    std::vector<std::pair<int,int>> playerPointVector_;

    //! String containing the animal moved with spinner
    std::string spinAnimal_;

    //! String containing the amount of moves for spinner animal
    std::string spinMoveCount_;

    //! Number for spins left in current gamephase
    int spinsLeft_;

};
}

#endif // GAMESTATE_HH
