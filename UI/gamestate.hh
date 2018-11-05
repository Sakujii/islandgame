#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"

namespace Student{

class GameState : public Common::IGameState
{
public:
    GameState() = default;
    ~GameState() = default;
};
}

#endif // GAMESTATE_HH
