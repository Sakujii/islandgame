#include "boardhex.hh"
#include "coordinateconvert.hh"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <qmath.h>

namespace Ui{

BoardHex::BoardHex(QGraphicsItem * parent): QGraphicsPolygonItem(parent)
{
    size_ = 15;

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

void BoardHex::drawHex(std::shared_ptr<Common::Hex> hexPtr, QGraphicsScene *boardScene)
{
    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    Common::CubeCoordinate cube = hexPtr->getCoordinates();

    QPointF axial = Student::cubeToAxial(cube, 15);

    boardScene->addItem(this);
    this->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    this->setToolTip(QString::number(cube.x) + "," + QString::number(cube.z));
    colorHex(hexPtr);
}

void BoardHex::colorHex(std::shared_ptr<Common::Hex> hexPtr)
{
    std::string type = hexPtr->getPieceType();
    if (type == "Water"){
        this->setBrush(QBrush(Qt::cyan));
    }
    else if (type == "Beach"){
        this->setBrush(QBrush(Qt::yellow));
    }
    else if (type == "Forest"){
        this->setBrush(QBrush(Qt::green));
    }
    else if (type == "Mountain"){
        this->setBrush(QBrush(Qt::lightGray));
    }
    else if (type == "Peak"){
        this->setBrush(QBrush(Qt::darkGray));
    }
    else {
        this->setBrush(QBrush(Qt::magenta));
    }
}

void BoardHex::focusInEvent(QFocusEvent*){
    this->setSelected(true);
    qDebug() << this->toolTip() << Q_FUNC_INFO;
}

void BoardHex::focusOutEvent(QFocusEvent*)
{
    qDebug() << this->toolTip() << Q_FUNC_INFO;
}


}
