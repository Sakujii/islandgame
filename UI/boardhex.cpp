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


void BoardHex::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::shared_ptr<Common::IGameRunner> game = win->getGame();
    try {
        game->flipTile(hexCoord_);
        colorHex();

        this->addActors();
        win->addBoardTransport(hexPtr_, this, boardPtr_);
    }
    catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
    }

    // Get pawns in hex before actor doAction
    std::vector<std::shared_ptr<Common::Pawn>> oldPawns = hexPtr_->getPawns();

    std::vector<Common::CubeCoordinate> neighbours = hexPtr_->getNeighbourVector();
    std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
    for (auto x : actors){
        if (x->getActorType() == "vortex"){
            for(auto x : neighbours){
                std::vector<std::shared_ptr<Common::Pawn>> pawns = boardPtr_->getHex(x)->getPawns();
                for (auto y : pawns){
                    oldPawns.push_back(y);
                }
            }
        }
        x->doAction();

        // Get pawns in hex after actor doAction
        std::vector<std::shared_ptr<Common::Pawn>> newPawns = hexPtr_->getPawns();
        if (x->getActorType() == "vortex"){
            for(auto x : neighbours){
                std::vector<std::shared_ptr<Common::Pawn>> pawns = boardPtr_->getHex(x)->getPawns();
                for (auto y : pawns){
                    newPawns.push_back(y);
                }
            }
        }
        // Delete pawns that were cleared from hex by actor
        for (auto x : oldPawns){
            if (std::find(newPawns.begin(), newPawns.end(), x) == newPawns.end()){
                int id = x->getId();
                boardPtr_->removePawn(id);
                win->removeBoardPawn(id);
            }
        }
    }
}


void BoardHex::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    QStringList data = event->mimeData()->text().split(";");
    int id = data[1].toInt();
    QString type = data[0];
    QString parentType = "hex";
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
                int capacity = -1;
                std::shared_ptr<Common::Pawn> pawnPtr;
                auto pawnIt = pawnMap.find(id);
                if (pawnIt != pawnMap.end()){
                    origin = pawnIt->second->getCoordinates();
                    pawnPtr = pawnIt->second;
                }


                // This needs Gamestates to be implemented
                // gamePtr_->movePawn(origin, hexCoord_, pawnId);

                // This is unneccessary when upper row is executed
                boardPtr_->movePawn(id, hexCoord_);

                auto iter = boardPawnMap.find(id);
                if (iter != boardPawnMap.end()){
                    iter->second->setParentItem(this);
                    iter->second->setPosition(hexPtr_->getPawnAmount(), parentType);
                }

                auto hexIt = hexMap.find(origin);
                if (hexIt != hexMap.end()){
                    // If pawn was in transport, remove it from there
                    if(hexIt->second->getTransports().size() != 0){
                        std::vector<std::shared_ptr<Common::Transport>> transports = hexIt->second->getTransports();
                        for(auto x : transports){
                            /*
                            std::vector<std::shared_ptr<Common::Pawn>> transportPawns = x->getPawnsInTransport();
                            for (auto y : transportPawns){
                                if (y == pawnPtr){
                                    x->removePawn(pawnPtr);
                                    parentType = "transport";
                                }
                            }*/
                            capacity = x->getCapacity();
                        }
                    }
                    // Get origin hex pawns and update positions
                    std::vector<std::shared_ptr<Common::Pawn>> oldPawns = hexIt->second->getPawns();
                    int i = 1;
                    if (parentType == "hex"){
                        for (auto x : oldPawns){
                            auto pawnIt = boardPawnMap.find(x->getId());
                            pawnIt->second->setPosition(i, parentType);
                            ++i;
                        }
                    }
                    else{
                        int j = 3;
                        for (auto x : oldPawns){
                            auto pawnIt = boardPawnMap.find(x->getId());
                            pawnIt->second->setPosition(j, parentType);
                            --j;
                        }
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

            }
        }

        catch (Common::IllegalMoveException& e){
            std::cout << e.msg() << std::endl;
        }
    }
}
