#include "boardhex.hh"

#include <QDebug>
#include <QBrush>
#include <qmath.h>


namespace Ui{

BoardHex::BoardHex(QGraphicsItem * parent): QGraphicsPolygonItem(parent)
{
    int size_ = 20;

    QPolygonF polygon;
    double dx = qSqrt(3)/2 * size_;
    polygon
            << QPointF(dx, -size_/2)
            << QPointF(0, -size_)
            << QPointF(-dx, -size_/2)
            << QPointF(-dx, size_/2)
            << QPointF(0, size_)
            << QPointF(dx, size_/2);

    this->setPolygon(polygon);

    setAcceptDrops(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

int BoardHex::getSize() const
{
    return size_;
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
