#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <map>
#include <unordered_map>

#include "igameboard.hh"

namespace Student{

class GameBoard : public Common::IGameBoard
{
public:
    GameBoard();
    ~GameBoard();

};
}

#endif // GAMEBOARD_HH
