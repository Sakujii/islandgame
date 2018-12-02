#include "boardtransport.hh"
#include "mainwindow.hh"
#include "illegalmoveexception.hh"
#include "transport.hh"

#include <QGraphicsScene>
#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QDebug>
#include <iostream>

namespace Ui {

BoardTransport::BoardTransport(QGraphicsItem *parent,
                               int id,
                               std::string transportType,
                               std::shared_ptr<Student::GameBoard> boardPtr,
                               std::shared_ptr<Common::IGameRunner> gamePtr): QGraphicsRectItem(parent)
{
    boardPtr_ = boardPtr;
    gamePtr_ = gamePtr;
    transportId_ = id;
    transportType_ = transportType;
    if (transportType_ == "boat"){
        brush_ = QBrush(QColor(110, 30, 5)); // Brown for boat
    } else if (transportType_ == "dolphin"){
        brush_ = QBrush(QColor(Qt::gray));
    } else{}

    drawTransport();
    // Draw transports on top of actors
    this->setZValue(1);
    setAcceptedMouseButtons(Qt::LeftButton);

    setAcceptDrops(true);
}

QRectF BoardTransport::boundingRect() const
{
    int ysize = 10;
    int xsize = 35;
    return QRectF(0, 0, xsize, ysize);
}

void BoardTransport::drawTransport()
{
    this->setPos(-18, 3);
    this->setRect(boundingRect());
    this->setBrush(brush_);
}


void BoardTransport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {

        QPixmap pixmap(boundingRect().size().toSize());
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setBrush(brush_);
        painter.drawRect(boundingRect());
        painter.end();

        QMimeData *mime = new QMimeData;
        QString data = QString::fromStdString(transportType_);
        data.append(";");
        data.append(QString::number(transportId_));
        mime->setText(data);
        QDrag *drag = new QDrag(win);
        drag->setMimeData(mime);
        drag->setHotSpot(QPoint(5, 7));
        drag->setPixmap(pixmap);
        drag->exec();
        delete drag;

    }
    this->show();
}

void BoardTransport::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    QStringList data = event->mimeData()->text().split(";");
    int id = data[1].toInt();
    QString type = data[0];

    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > hexMap = boardPtr_->getHexMap();
    std::unordered_map<int, std::shared_ptr<Common::Transport>> transportMap = boardPtr_->getTransportMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();
    std::map<Common::CubeCoordinate, Ui::BoardHex*> boardHexMap = win->getBoardHexMap();



    Common::CubeCoordinate coord;
    int capacity = -1;

    auto transportIt = transportMap.find(transportId_);
    if (transportIt != transportMap.end()){
        coord = transportIt->second->getHex()->getCoordinates();
        capacity = (transportIt->second->getCapacity());
    }

        try {
            if (type == "pawn"){
                if (capacity <= 0){
                    throw Common::IllegalMoveException("The transport is full!") ;
                }
                // Get pawn origin coordinates from pawn map
                Common::CubeCoordinate origin;
                auto pawnIt = pawnMap.find(id);
                std::shared_ptr<Common::Pawn> pawn = pawnIt->second;
                if (pawnIt != pawnMap.end()){
                    origin = pawn->getCoordinates();
                }

                std::vector<std::shared_ptr<Common::Pawn>> pawns = transportIt->second->getPawnsInTransport();
                for (auto x : pawns){
                    if (x == pawn){
                        throw Common::IllegalMoveException("Moving pawn inside the same transport!") ;
                    }
                }

                gamePtr_->movePawn(origin, coord, pawn->getId());

                transportIt->second->addPawn(pawnIt->second);

                auto iter = boardPawnMap.find(id);
                BoardPawn* boardPawn;
                if (iter != boardPawnMap.end()){
                    boardPawn = iter->second;
                    boardPawn->setParentItem(this);
                    boardPawn->setInTransport(true);
                    boardPawn->setPosition(capacity);
                }

                // Get origin hex pawns and update positions
                auto hexIt = hexMap.find(origin);
                auto boardHexIt = boardHexMap.find(origin);
                if (boardHexIt != boardHexMap.end()){
                    boardHexIt->second->reArrangePawns(hexIt->second);
                }
            }
        }
        catch (Common::IllegalMoveException& e){
            std::cout << e.msg() << std::endl;
            win->setGameMessage(e.msg());
        }
}

}
