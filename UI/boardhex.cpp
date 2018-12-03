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
                   std::shared_ptr<Common::IGameRunner> gamePtr):
    QGraphicsPolygonItem(parent)
{
    hexPtr_ = hexPtr;
    boardPtr_ = boardPtr;
    hexCoord_ = hexPtr->getCoordinates();
    gamePtr_ = gamePtr;
    size_ = 35;

    QPolygonF polygon;
    double dx = (qSqrt(3))/2 * size_;
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
    } else{}
}

void BoardHex::actorAction(std::shared_ptr<Common::Actor> actor)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();

    // Get pawns and transports in hex before actor doAction
    std::vector<std::shared_ptr<Common::Pawn>> pawnsBefore = hexPtr_->getPawns();
    std::vector<std::shared_ptr<Common::Pawn>> pawnsAfter;
    std::vector<std::shared_ptr<Common::Transport>> transportsBefore = hexPtr_->getTransports();
    std::vector<std::shared_ptr<Common::Transport>> transportsAfter;

    // If actor is vortex, also neighbour hexes have to be checked
    if (actor->getActorType() == "vortex"){
        std::vector<Common::CubeCoordinate> neighbours = hexPtr_->getNeighbourVector();
        for(auto x : neighbours){
            std::vector<std::shared_ptr<Common::Pawn>> pawns = boardPtr_->getHex(x)->getPawns();
            for (auto y : pawns){
                pawnsBefore.push_back(y);
            }
            std::vector<std::shared_ptr<Common::Transport>> transports = boardPtr_->getHex(x)->getTransports();
            for (auto y : transports){
                transportsBefore.push_back(y);
            }
        }
        actor->doAction();

        pawnsAfter = hexPtr_->getPawns();
        transportsAfter = hexPtr_->getTransports();
        for(auto x : neighbours){
            std::vector<std::shared_ptr<Common::Pawn>> pawns = boardPtr_->getHex(x)->getPawns();
            std::vector<std::shared_ptr<Common::Transport>> transports = boardPtr_->getHex(x)->getTransports();
            for (auto y : transports){
                transportsAfter.push_back(y);
            }
            for (auto z : pawns){
                pawnsAfter.push_back(z);
            }
        }

        // Remove also vortex itself
        win->removeBoardActor(actor->getId());
        win->setGameMessage("Vortex destroyed everything around it!");
    }

    else{
    actor->doAction();

    // Get pawns and transports in hex after actor doAction
    pawnsAfter = hexPtr_->getPawns();
    transportsAfter = hexPtr_->getTransports();
    }

    // Delete pawns that were cleared from hex by actor
    for (auto x : pawnsBefore){
        if (std::find(pawnsAfter.begin(), pawnsAfter.end(), x) == pawnsAfter.end()){
            int id = x->getId();
            boardPtr_->removePawn(id);
            win->removeBoardPawn(id);
        }
    }
    // Delete transports that were cleared from hex by actor
    for (auto x : transportsBefore){
        if (std::find(transportsAfter.begin(), transportsAfter.end(), x) == transportsAfter.end()){
            int id = x->getId();
            boardPtr_->removeTransport(id);
            win->removeBoardTransport(id);
        }
    }
}

void BoardHex::reArrangePawns(std::shared_ptr<Common::Hex> hex)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();

    std::vector<std::shared_ptr<Common::Pawn>> pawns = hex->getPawns();
    std::vector<std::shared_ptr<Common::Transport>> transports = hex->getTransports();
    std::vector<std::shared_ptr<Common::Pawn>> transportPawns;

    int i = 1;
    int j = 3;
    for (auto x : pawns){
        auto pawnIt = boardPawnMap.find(x->getId());
        BoardPawn *boardPawn = pawnIt->second;
        if (boardPawn->getInTransport()){
            boardPawn->setPosition(j);
            --j;
        }
        else{
            boardPawn->setPosition(i);
            ++i;
        }
    }
}


void BoardHex::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Student::MainWindow *win = Student::MainWindow::getInstance();

    try {
        if(gamePtr_->currentGamePhase() == Common::GamePhase::SINKING)
        {
            gamePtr_->flipTile(hexCoord_);
            colorHex();

            win->addBoardActor(hexPtr_, this);
            win->addBoardTransport(hexPtr_, this, boardPtr_);
            win->nextGamephase();
        }
    }
    catch (Common::GameException& e) {
        std::cout<< e.msg() <<std::endl;
        win->setGameMessage(e.msg());
    }

    std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
    for (auto x : actors){
        actorAction(x);
    }
}


void BoardHex::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    QStringList data = event->mimeData()->text().split(";");
    int id = data[1].toInt();
    QString type = data[0];

    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > hexMap = boardPtr_->getHexMap();
    std::unordered_map<int, std::shared_ptr<Common::Transport>> transportMap = boardPtr_->getTransportMap();
    std::unordered_map<int, std::shared_ptr<Common::Actor>> actorMap = boardPtr_->getActorMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();
    std::unordered_map<int, Ui::BoardTransport*> boardTransportMap = win->getBoardTransportMap();
    std::unordered_map<int, Ui::BoardActor*> boardActorMap = win->getBoardActorMap();
    std::shared_ptr<Student::GameState> state = win->getState();
    std::string actortype = "0";
    auto actor = actorMap.find(id);
    if(actor != actorMap.end())
    {
        std::shared_ptr<Common::Actor> actor = actorMap.at(id);
        actortype = actor->getActorType();
    }

        try {
            if (type == "pawn" && gamePtr_->currentGamePhase()==Common::GamePhase::MOVEMENT){
                // Get pawn origin coordinates from pawn map
                Common::CubeCoordinate origin;
                std::shared_ptr<Common::Pawn> pawnPtr;
                auto pawnIt = pawnMap.find(id);
                if (pawnIt != pawnMap.end()){
                    origin = pawnIt->second->getCoordinates();
                    pawnPtr = pawnIt->second;
                }

                gamePtr_->movePawn(origin, hexCoord_, id);
                // Update actions left label
                win->numberOfActionsLeft(gamePtr_->getCurrentPlayer()->getActionsLeft());

                auto boardPawnIt = boardPawnMap.find(id);
                BoardPawn* boardPawn;
                if (boardPawnIt != boardPawnMap.end()){
                    boardPawn = boardPawnIt->second;
                    boardPawn->setParentItem(this);
                }
                if(hexPtr_->getPieceType() == "Coral")
                {
                    state->addPointsToPlayer(state->currentPlayer(), 10);
                    hexPtr_->clearPawnsFromTerrain();
                    win->removeBoardPawn(id);
                    win->updatePointsList();
                    win->setGameMessage("Pawn rescued! Points added");
                }

                auto hexIt = hexMap.find(origin);
                if (hexIt != hexMap.end()){
                    // If pawn was in transport, remove it from there
                    std::vector<std::shared_ptr<Common::Transport>> transports =
                            hexIt->second->getTransports();
                    if (boardPawn->getInTransport() == true){
                        for (auto x : transports){
                            x->removePawn(pawnPtr);
                        }
                    }
                    boardPawn->setInTransport(false);
                    reArrangePawns(hexPtr_);
                    reArrangePawns(hexIt->second);
                }
            }
            else if ((type == "boat" || type == "dolphin")){
                if(hexPtr_->getTransports().size() > 0){
                    throw Common::IllegalMoveException
                            ("There is already a transport in the hex!");
                }
                // Get transport origin coordinates from transport map
                Common::CubeCoordinate origin;
                auto transportIt = transportMap.find(id);
                if (transportIt != transportMap.end()){
                    origin = transportIt->second->getHex()->getCoordinates();
                }

                if(gamePtr_->currentGamePhase()==Common::GamePhase::MOVEMENT)
                {
                    gamePtr_->moveTransport(origin, hexCoord_, id);
                }
                else if(gamePtr_->currentGamePhase()==Common::GamePhase::SPINNING && type == "dolphin")
                {
                    gamePtr_->moveTransportWithSpinner(origin, hexCoord_, id, state->getSpinMovecount());
                }
                auto iter = boardTransportMap.find(id);
                if (iter != boardTransportMap.end()){
                    iter->second->setParentItem(this);
                }
            }
            else if(actortype == state->getSpinAnimal()){
                if(hexPtr_->getActors().size() > 0){
                    throw Common::IllegalMoveException
                            ("There is already an actor in the hex!");
                }

                // Get actor origin coordinates from actor map
                Common::CubeCoordinate origin;
                auto actorIt = actorMap.find(id);
                if (actorIt != actorMap.end()){
                    origin = actorIt->second->getHex()->getCoordinates();
                }

                gamePtr_->moveActor(origin, hexCoord_, id, state->getSpinMovecount());

                auto iter = boardActorMap.find(id);
                if (iter != boardActorMap.end()){
                    iter->second->setParentItem(this);
                }
            } else {}

            // Execute actor actions after every drop to hex
            std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr_->getActors();
            for (auto x : actors){
                actorAction(x);
            }
        }

        catch (Common::IllegalMoveException& e){
            win->setGameMessage(e.msg());
        }
}
}
