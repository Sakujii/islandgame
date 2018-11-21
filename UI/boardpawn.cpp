#include "boardpawn.hh"

#include <QGraphicsScene>
#include <QDrag>


namespace Ui{

BoardPawn::BoardPawn(QGraphicsItem* parent): QGraphicsEllipseItem (parent)
{


    setAcceptedMouseButtons(Qt::LeftButton);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

void BoardPawn::drawPawn()
{
    this->setRect(0,0, 10, 10);
    this->setBrush(QBrush(Qt::black));
}

void BoardPawn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{/* //Not working for some reason
    if (event->button() == Qt::LeftButton) {
        QDrag *drag = new QDrag(event->widget());
        Qt::DropAction dropAction = drag->exec();

    }*/
}

}
