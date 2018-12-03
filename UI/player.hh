#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"

namespace Student{

class Player : public Common::IPlayer
{
public:

    /**
     * @brief Constructor
     * @param id Identification number of the player
     * @post Private variables are initialized
     */
    explicit Player(int id);

    /**
     * @brief Default destructor
     */
    ~Player() = default;

    /**
     * @brief getPlayer gets the player id.
     * @return The identifier of the player.
     * @post Exception quarantee: nothrow
     */
    int getPlayerId() const;

    /**
     * @brief setActionsLeft sets the player's remaining number of actions.
     * @param actionsLeft The number of actions left.
     * @post Actions left set to parameter value. Exception quarantee: nothrow
     */
    void setActionsLeft(unsigned int actionsLeft);

    /**
     * @brief getActionsLeft returns the amount of actions left for the player
     * @return amount of actions
     * @post Exception quarantee: nothrow
     */
    unsigned int getActionsLeft() const;

private:
    //! Identification number of the player
    int playerid_;

    //! Number of actions left on current gamephase
    unsigned int actionsLeft_;

};
}

#endif // PLAYER_HH
