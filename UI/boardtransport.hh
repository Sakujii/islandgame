#ifndef BOARDTRANSPORT_HH
#define BOARDTRANSPORT_HH

#include "gameboard.hh"
#include "igamerunner.hh"

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <QMimeData>
#include <memory>

/**
 * @file
 * @brief Implements graphic class BoardPawn
 */

namespace Ui {

/**
 * @brief Offers a graphical class for transports.
 * Graphical transports are moved on the board with drag
 * and drop actions. There are two types of transports:
 * boat and dolphin. Boat takes three pawns and dolphin
 * takes one pawn.
 */
class BoardTransport: public QGraphicsRectItem
{
public:
    /**
     * @brief Constructor
     * @param parent Parent GraphicsItem for transport
     * @param id Identification number for transport
     * @param transportType Type of the transport as string
     * @param board Shared pointer to gameboard object
     * @param game Shared pointer to game object
     */
    explicit BoardTransport(QGraphicsItem *parent = 0,
                            int id = 0,
                            std::string transportType = "",
                            std::shared_ptr<Student::GameBoard> board = nullptr,
                            std::shared_ptr<Common::IGameRunner> game = nullptr);

    /**
    * @brief Default destructor
    */
    ~BoardTransport() = default;

    /**
     * @brief boundingRect gets the size of transport item rect
     * @return returns the bounding rectangle of the transport as QRectF
     * @post Exception guarantee: no throw
     */
    QRectF boundingRect() const override;

    /**
     * @brief drawTransport draws the transport graphics on the board
     * @post Transport is colored and drawn on the board
     * @post Exception guarantee: strong
     */
    void drawTransport();


private:
    //! Transport identification number
    int transportId_;

    //! Transport type
    std::string transportType_;

    //! Shared pointer to gameboard object
    std::shared_ptr<Student::GameBoard> boardPtr_;

    //! Shared pointer to igamerunner object
    std::shared_ptr<Common::IGameRunner> gamePtr_;

    //! Qt Brush to color the transport with
    QBrush brush_;

    /**
     * @brief overrided QT mousePressEvent tracks clicking of transport graphics
     * @param event QT scene mouse event
     * @post Drag event sends valid mimedata to item transport was dropped to
     * @post Exception quarantee: strong
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief overrided QT dropEvent tracks drops to transport graphics
     * @param QT scene mouse event
     * @post State of an object dropped succesfully to transport is changed
     * @throw Common::IllegalMoveException if illegal move was attempted
     * @post Exception quarantee: basic
     */
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
};

}

#endif // BOARDTRANSPORT_HH
