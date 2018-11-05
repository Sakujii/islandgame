#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"

namespace Student{

class Player : public Common::IPlayer
{
public:
    Player() = default;
    ~Player() = default;
};
}

#endif // PLAYER_HH
