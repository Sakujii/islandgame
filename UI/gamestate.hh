#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"

namespace Student{

class GameState : public Common::IGameState
{
public:
    GameState();
    ~GameState();
};
}

#endif // GAMESTATE_HH
