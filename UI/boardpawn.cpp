#include "boardpawn.hh"

#include <QGraphicsScene>

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

}
