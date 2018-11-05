#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <map>
#include <unordered_map>

#include "igameboard.hh"

/**
 * @file
 * @brief GameBoard class for the gameboard
 */

namespace Student{

class GameBoard : public Common::IGameBoard
{
public:
    GameBoard();
    ~GameBoard();

};
}

#endif // GAMEBOARD_HH
