#include "boardhex.hh"
#include "coordinateconvert.hh"
#include "mainwindow.hh"
#include "gameexception.hh"
#include "boardpawn.hh"
#include "actor.hh"
#include "transport.hh"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <qmath.h>
#include <iostream>
#include <string>



namespace Ui{

BoardHex::BoardHex(QGraphicsItem * parent): QGraphicsPolygonItem(parent)
{
    size_ = 18;

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

    if (hexCoord_.x < 2 && hexCoord_.x > -2 && hexCoord_.z < 2 && hexCoord_.z > -2){
        BoardPawn * boardPawn = new BoardPawn(this);
        //boardPawn->drawPawn();
        //boardPawn->setPos(0, 0);
    }
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

        // Get actors under the tile and draw first letter on the hex
        std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
        for (auto x : actors){
            std::string text = (x->getActorType());
            text = std::toupper(text[0]);
            QString qtext = QString::fromStdString(text);
            QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(qtext, this);
            textItem->setFont(QFont("Colibri", 25));
            textItem->setPos(-7, -15);
        }

        // Get transports under the tile and draw first letter on the hex
        std::vector<std::shared_ptr<Common::Transport>> transports = hexPtr_->getTransports();
        for (auto x : transports){
            std::string text = (x->getTransportType());
            text = std::toupper(text[0]);
            QString qtext = QString::fromStdString(text);
            QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(qtext, this);
            textItem->setFont(QFont("Colibri", 25));
            textItem->setPos(-10, -15);
        }
    }
    catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
    }

}


void BoardHex::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    qDebug() << "Got a drop! " << hexCoord_.x << hexCoord_.z ;

}
}
