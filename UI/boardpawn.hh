#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QGraphicsEllipseItem>
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
 * @brief Offers a graphical class for pawns.
 * Graphical pawns are moved on the board with drag
 * and drop actions. Each player has their own different
 * colored pawns.
 */
class BoardPawn: public QGraphicsEllipseItem
{

public:
    /**
     * @brief Constructor
     * @param parent Parent GraphicsItem for hex
     * @param id Identification number for pawn
     * @param playerId Identification number of player owning the pawn
     */
    explicit BoardPawn(QGraphicsItem *parent = 0, int id = 0, int playerId = 0);

    /**
    * @brief Default destructor
    */
    ~BoardPawn() = default;

    /**
     * @brief boundingRect gets the size of pawn item rect
     * @return returns the bounding rectangle of the pawn as QRectF
     * @post Exception guarantee: no throw
     */
    QRectF boundingRect() const override;

    /**
     * @brief drawPawn draws the pawn graphics on the board
     * @post Pawn is colored and drawn on the board
     * @post Exception guarantee: strong
     */
    void drawPawn();

    /**
     * @brief setPosition sets the position of the pawn
     * @post Pawn is set on the right position in the hex
     * @note Different positions for pawns in transport
     * @post Exception guarantee: strong
     */
    void setPosition(int);

    /**
     * @brief setInTransport sets private variable inTransport_
     * @param inTransport value as bool
     * @post inTransport_ is set to parameter value
     * @post Exception quarantee: strong
     */
    void setInTransport(bool);

    /**
     * @brief getInTransport gets private variable inTransport_ value
     * @return inTransport_ value as bool
     * @post Exception quarantee: no throw
     */
    bool getInTransport();


private:
    //! Identification number of the pawn
    int pawnId_;

    //! Identification number of the player owning the pawn
    int playerId_;

    //! Qt brush to color the pawn with
    QBrush brush_;

    //! Bool variable which tells if pawn is in transport
    bool inTransport_;

    /**
     * @brief overrided QT mousePressEvent tracks clicking of pawn graphics
     * @param event QT scene mouse event
     * @post Drag event sends valid mimedata to item pawn was dropped to
     * @post Exception quarantee: strong
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

}

#endif // BOARDPAWN_HH
