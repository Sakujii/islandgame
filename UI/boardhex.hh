#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <hex.hh>
#include <gameboard.hh>
#include <boardpawn.hh>
#include <igamerunner.hh>

#include <QtWidgets/QGraphicsPolygonItem>
#include <memory>
#include <map>
#include <unordered_map>

/**
 * @file
 * @brief Implements graphic class BoardHex
 */

namespace Ui{

/**
 * @brief Offers an graphical class for hexes.
 * The gameboard is based on hex shaped tiles and everything
 * else is drawn on top of these hexes.
 */
class BoardHex :public QGraphicsPolygonItem, public std::enable_shared_from_this<BoardHex>
{

public:
    /**
     * @brief Constructor.
     * @param parent Parent GraphicsItem for hex
     * @param hex Shared pointer to hex object
     * @param board Shared pointer to gameboard object
     * @param game Shared pointer to gameengine object
     */
    explicit BoardHex(QGraphicsItem *parent = 0,
                      std::shared_ptr<Common::Hex> hex = nullptr,
                      std::shared_ptr<Student::GameBoard> board = nullptr,
                      std::shared_ptr<Common::IGameRunner> game = nullptr);
    /**
     * @brief getSize gets the size of graphical hex
     * @return returns size of hex as int
     * @post Exception guarantee: no throw
     */
    int getSize() const;

    /**
     * @brief colorHex sets a color to the hex according it's type
     * @post Exception guarantee: strong
     */
    void colorHex();

    /**
     * @brief actorAction executes action of the actor
     * @param actor Shared pointer to actor object that is acting
     * @post Pawns and transports destroyed by actor are removed from game
     * @post Actor is removed from game if needed
     * @post Exception guarantee: strong
     */
    void actorAction(std::shared_ptr<Common::Actor> actor);

    /**
     * @brief reArrangePawns arranges the pawns on the hex
     * @param hex Shared pointer to hex object
     * @post Pawns are rearranged to their spots
     * @post Exception guarantee: strong
     */
    void reArrangePawns(std::shared_ptr<Common::Hex> hex);

    /**
     * @brief overrided QT mousepressevent tracks clicking of hex graphics
     * @param event QT scene mouse event
     * @post Hex is flipped if needed and actor action is called
     * @throw Common::IllegalMoveException if illegal move was attempted
     * @post Exception quarantee: strong
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief overrided QT dropEvent tracks drops to hex graphics
     * @param QT scene mouse event
     * @post State of any object dropped to hex is changed
     * @post If there was actor in hex, actor acts
     * @throw Common::IllegalMoveException if illegal move was attempted
     * @post Exception quarantee: basic
     */
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    int size_;
    Common::CubeCoordinate hexCoord_;
    std::shared_ptr<Common::Hex> hexPtr_;
    std::shared_ptr<Student::GameBoard> boardPtr_;
    std::shared_ptr<Common::IGameRunner> gamePtr_;
};
}

#endif // BOARDHEX_HH
