#include "player.hh"

namespace Student{

Player::Player(int id)
{
    _playerid=id;
    _actionsLeft = 0;
    _pawnLeft = 1;
    _totalPawnCount = 1;
}

Player::~Player(){}

int Player::getPlayerId()const
{
    return _playerid;
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    _actionsLeft = actionsLeft;
}

unsigned int Player::getActionsLeft() const
{
    return _actionsLeft;
}


}
