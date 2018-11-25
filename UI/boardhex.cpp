#include "boardhex.hh"
#include "coordinateconvert.hh"
#include "mainwindow.hh"
#include "gameexception.hh"
#include "actor.hh"
#include "transport.hh"
#include "pawn.hh"
#include "illegalmoveexception.hh"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QMimeData>
#include <qmath.h>
#include <iostream>
#include <string>



namespace Ui{


BoardHex::BoardHex(QGraphicsItem * parent,
                   std::shared_ptr<Common::Hex> hexPtr,
                   std::shared_ptr<Student::GameBoard> boardPtr,
                   std::shared_ptr<Common::IGameRunner> gamePtr): QGraphicsPolygonItem(parent)
{
    hexPtr_ = hexPtr;
    boardPtr_ = boardPtr;
    hexCoord_ = hexPtr->getCoordinates();
    gamePtr_ = gamePtr;
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

void BoardHex::drawHex(std::shared_ptr<Common::Hex> hexPtr,
                       QGraphicsScene *boardScene,
                       std::shared_ptr<Student::GameBoard> boardPtr)
{
    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    hexCoord_ = hexPtr_->getCoordinates();

    QPointF axial = Student::cubeToAxial(hexCoord_, size_);

    boardScene->addItem(this);
    this->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    this->setToolTip(QString::number(hexCoord_.x) + "," + QString::number(hexCoord_.z));
    colorHex();

    std::vector<std::shared_ptr<Common::Pawn>> pawns = hexPtr_->getPawns();

    int i = 1;
    for (auto x : pawns){
        BoardPawn *boardPawn = new BoardPawn(this, x->getId(), x->getPlayerId());
        if (i == 1){
            boardPawn->setPos(-5, -15);
        } else if (i == 2){
            boardPawn->setPos(-13, -5);
        } else if (i == 3){
            boardPawn->setPos(3, -5);
        } ++i;
        if (boardPawnMap_.find(x->getId()) == boardPawnMap_.end()){
            std::cout << x->getId() << std::endl;
            std::cout << boardPawn << std::endl;
            boardPawnMap_.insert(std::make_pair(x->getId(), boardPawn));
            std::cout << boardPawnMap_.size()<<std::endl;
        }
    }

    boardPtr_ = boardPtr;

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
    else{}
}

void BoardHex::addActors()
{
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
}

void BoardHex::addTransports()
{
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

void BoardHex::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::shared_ptr<Common::IGameRunner> game = win->getGame();
    try {
        game->flipTile(hexCoord_);
        colorHex();

        this->addActors();
        this->addTransports();
    }
    catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
    }

}


void BoardHex::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    int pawnId = event->mimeData()->text().toInt();
    qDebug() << "Got a drop to" << hexCoord_.x << hexCoord_.z << "from pawnId" << pawnId;

    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > hexMap = boardPtr_->getHexMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();


    // Get pawn origin coordinates from pawn map
    Common::CubeCoordinate origin;
    auto pawnIt = pawnMap.find(pawnId);
    if (pawnIt != pawnMap.end()){
        origin = pawnIt->second->getCoordinates();
    }

    try {
        // This needs player instances in playerVector
        // gamePtr_->movePawn(origin, hexCoord_, pawnId);

        // This is unneccessary when upper row is executed
        boardPtr_->movePawn(pawnId, hexCoord_);

        auto iter = boardPawnMap.find(pawnId);
        if (iter != boardPawnMap.end()){
            iter->second->setParentItem(this);
            iter->second->setPosition(hexPtr_->getPawnAmount());
        }

        // Get origin hex pawns and update positions
        auto hexIt = hexMap.find(origin);
        if (hexIt != hexMap.end()){
            std::vector<std::shared_ptr<Common::Pawn>> oldPawns = hexIt->second->getPawns();
            int i = 1;
            for (auto x : oldPawns){
                auto pawnIt = boardPawnMap.find(x->getId());
                pawnIt->second->setPosition(i);
                ++i;
            }
        }
    }
    catch (Common::IllegalMoveException& e){
        std::cout << e.msg() << std::endl;
    }

}
}
