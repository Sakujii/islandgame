#include "player.hh"

namespace Student{

Player::Player(int id)
{
    playerid_ = id;
    actionsLeft_ = 3;
}

int Player::getPlayerId()const
{
    return playerid_;
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    actionsLeft_ = actionsLeft;
}

unsigned int Player::getActionsLeft() const
{
    return actionsLeft_;
}


}
