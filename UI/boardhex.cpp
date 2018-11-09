#include "boardhex.hh"

#include <QDebug>
#include <QBrush>

namespace Ui{

BoardHex::BoardHex(QGraphicsItem * parent): QGraphicsPolygonItem(parent)
{
    setAcceptDrops(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);

}

void BoardHex::focusInEvent(QFocusEvent *){
    qDebug() << this->toolTip() << Q_FUNC_INFO;
}

void BoardHex::focusOutEvent(QFocusEvent*)
{
    qDebug() << this->toolTip() << Q_FUNC_INFO;
}

void BoardHex::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    this->setBrush(QBrush(Qt::cyan));
}

void BoardHex::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    this->setBrush(QBrush());
}


}
