#ifndef BOARDACTOR_HH
#define BOARDACTOR_HH


#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMimeData>

/**
 * @file
 * @brief Implements graphic class BoardActor
 */

namespace Ui {

/**
 * @brief Offers an graphical class for
 * different actors. Actors roam around the
 * gameboard and cause actions whenever their location hex tiles are flipped.
 */
class BoardActor: public QGraphicsPolygonItem
{

public:
    /**
     * @brief Constructor.
     * @param parent Parent GraphicsItem for actor
     * @param id Identification number for actor
     * @param actorType Type of actor
     */
    explicit BoardActor(QGraphicsItem *parent = 0,
                            int id = 0,
                            std::string actorType = "");

    /**
     * @brief Default destructor
     */
    ~BoardActor() = default;

    /**
     * @brief drawActor draws the graphical actor on board
     * @post Exception quarantee: strong
     */
    void drawActor();

    /**
     * @brief overrided QT mousepressevent tracks clicking of actor graphics
     * @param event QT scene mouse event
     * @post Drag event sends valid mimedata to item where drop was done
     * @post Exception quarantee: strong
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int size_;
    int actorId_;
    QBrush brush_;
    std::string actorType_;
    QPolygonF polygon_;
    QGraphicsSimpleTextItem *textItem_;
};

}

#endif // BOARDACTOR_HH
