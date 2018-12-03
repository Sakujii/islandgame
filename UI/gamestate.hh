#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"
#include <vector>
#include <iostream>

namespace Student{


class GameState : public Common::IGameState
{
public:
    /**
     * @brief Constructor, exists solely for documentation.
     */
    explicit GameState();

    /**
     * @brief Virtual destructor. Does nothing, since this is an interface class.
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
     * @brief initPoints initializes playerpointvector.
     * @param playercount number of players.
     */
    void initPoints(int playercount);
    /**
     * @brief getPointsVectorForPlot transforms pointvector to strings
     * @return vector containing strings of player and points
     */
    std::vector<std::string> getPointsVectorForPlot();
    /**
     * @brief currentGamePhaseString transforms current gamephase to string
     * @return string of the gamephase
     */
    std::string currentGamePhaseString();
    /**
     * @brief setSpinresult saves spinresult to object
     * @param pair of spinresult animal, movecount
     */
    void setSpinResult(std::pair<std::string, std::string> spinresult);
    /**
     * @brief getSpinMovecount returns spinresult movecount
     * @return string of animal
     */
    std::string getSpinMovecount();
    /**
     * @brief getSpinMoveAnimal returns spinresult animal
     * @return string of animal
     */
    std::string getSpinAnimal();
    /**
     * @brief getWinner finds player with most points and return result
     * @return string of winnertext
     */
    std::string getWinner();
    /**
     * @brief getSpinsLeft returns spins left
     * @return amount of spins as int
     */
    int getSpinsLeft();
    /**
     * @brief setSpinsLeft sets spins left
     * @param amount of spins as int
     */
    void setSpinsLeft(int spincount);
    /**
     * @brief getPlayerPoints gets players' points
     * @param playerid int
     * @return amount of points player has as int
     */
    int getPlayerPoints(int playerid);


private:
    Common::GamePhase gamePhaseId_;
    int playerInTurn_;
    std::vector<std::pair<int,int>> playerPointVector_;
    std::string spinAnimal_;
    std::string spinMoveCount_;
    int spinsLeft_;

};
}

#endif // GAMESTATE_HH
