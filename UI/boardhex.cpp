#include "boardhex.hh"
#include "coordinateconvert.hh"
#include "mainwindow.hh"
#include "gameexception.hh"
#include "boardpawn.hh"
#include "actor.hh"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <qmath.h>
#include <iostream>

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
    hexPtr_ = hexPtr;
    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    hexCoord_ = hexPtr_->getCoordinates();

    QPointF axial = Student::cubeToAxial(hexCoord_, size_);

    boardScene->addItem(this);
    this->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    this->setToolTip(QString::number(hexCoord_.x) + "," + QString::number(hexCoord_.z));
    colorHex();

    //std::shared_ptr<BoardPawn> boardPawn = std::make_shared<BoardPawn>();
    BoardPawn * boardPawn = new BoardPawn(this);
    boardPawn->drawPawn();
    boardPawn->setPos(0, 0);
}

void BoardHex::colorHex()
{
    std::string type = hexPtr_->getPieceType();
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
    else if (type == "Coral") {
        this->setBrush(QBrush(Qt::magenta));
    }
}

void BoardHex::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::shared_ptr<Common::IGameRunner> game = win->getGame();
    try {
        game->flipTile(hexCoord_);
        colorHex();
    }
    catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
    }
    std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
    for (auto x : actors){
        std::cout << x->getActorType() << std::endl;
    }
}

}
