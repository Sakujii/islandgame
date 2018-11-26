#include "boardhex.hh"
#include "coordinateconvert.hh"
#include "mainwindow.hh"
#include "gameexception.hh"
#include "actor.hh"
#include "transport.hh"
#include "pawn.hh"
#include "illegalmoveexception.hh"
#include "boardtransport.hh"

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
    Student::MainWindow *win = Student::MainWindow::getInstance();

    // Get transport under the tile and create transport instance
    std::vector<std::shared_ptr<Common::Transport>> transports = hexPtr_->getTransports();
    for (auto x : transports){
        std::string text = (x->getTransportType());
        BoardTransport *boardTransport = new BoardTransport(this, x->getId(), x->getTransportType());

    }
}

void BoardHex::removePawns()
{
    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();

    // If pawn does not exist in pawnmap, delete boardpawn
    /*
    for (auto x : boardPawnMap){
        auto pawnIt = pawnMap.find(x.first);
        if (pawnIt == pawnMap.end()){
            auto boardPawnIt = boardPawnMap.find(x.first);
            delete(x.second);
            boardPawnMap.erase(boardPawnIt);
        }
    }*/

    for (auto it = boardPawnMap.cbegin(); it != boardPawnMap.cend();)
    {
        auto pawnIt = pawnMap.find(it->first);
        if (pawnIt == pawnMap.end()){
            it = boardPawnMap.erase(it);
            delete(it->second);
            std::cout <<"kilikiki" << std::endl;
        } else {
        ++it;
        }
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

    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    //std::cout << pawnMap.size() << std::endl;

    std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
    for (auto x : actors){
        x->doAction();
        // Do we really have to call gameboards removepawn for actor action?!
    }

    std::vector<std::shared_ptr<Common::Pawn>> pawns = hexPtr_->getPawns();

    pawnMap = boardPtr_->getPawnMap();
    //std::cout << pawnMap.size() << std::endl;

    removePawns();

}


void BoardHex::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    QStringList data = event->mimeData()->text().split(";");
    int id = data[1].toInt();
    QString type = data[0];
    qDebug() << "Got a drop to" << hexCoord_.x << hexCoord_.z << "from" << type << "id" << id;

    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > hexMap = boardPtr_->getHexMap();
    std::unordered_map<int, std::shared_ptr<Common::Transport>> transportMap = boardPtr_->getTransportMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();
    std::unordered_map<int, Ui::BoardTransport*> boardTransportMap = win->getBoardTransportMap();

        try {
            if (type == "pawn"){
                // Get pawn origin coordinates from pawn map
                Common::CubeCoordinate origin;
                auto pawnIt = pawnMap.find(id);
                if (pawnIt != pawnMap.end()){
                    origin = pawnIt->second->getCoordinates();
                }

                // This needs Gamestates to be implemented
                // gamePtr_->movePawn(origin, hexCoord_, pawnId);

                // This is unneccessary when upper row is executed
                boardPtr_->movePawn(id, hexCoord_);

                auto iter = boardPawnMap.find(id);
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
            else{
                // Get transport origin coordinates from transport map
                Common::CubeCoordinate origin;
                auto transportIt = transportMap.find(id);
                if (transportIt != transportMap.end()){
                    origin = transportIt->second->getHex()->getCoordinates();
                }

                // This needs Gamestates to be implemented
                // gamePtr_->moveTransport(origin, hexCoord_, id);

                // This is unneccessary when upper row is executed
                boardPtr_->moveTransport(id, hexCoord_);

                auto iter = boardTransportMap.find(id);
                if (iter != boardTransportMap.end()){
                    iter->second->setParentItem(this);
                }

                // Get pawns on transport and update positions
                std::vector<std::shared_ptr<Common::Pawn>> pawns = transportIt->second->getPawnsInTransport();
                int i = 1;
                for (auto x : pawns){
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
