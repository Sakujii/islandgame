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

QColor color4 = QColor(110, 30, 5);
QColor color5 = Qt::gray;

BoardTransport::BoardTransport(QGraphicsItem *parent,
                               int id,
                               std::string transportType,
                               std::shared_ptr<Student::GameBoard> boardPtr): QGraphicsRectItem(parent)
{
    boardPtr_ = boardPtr;
    transportId_ = id;
    transportType_ = transportType;
    drawTransport();
    setAcceptedMouseButtons(Qt::LeftButton);
    this->setPos(-13, 3);
    setAcceptDrops(true);

}

QRectF BoardTransport::boundingRect() const
{
    int ysize = 7;
    int xsize = 25;
    return QRectF(0, 0, xsize, ysize);
}

void BoardTransport::drawTransport()
{
    this->setRect(boundingRect());

    if (transportType_ == "boat"){
        this->setBrush(QBrush(color4));
    } else if (transportType_ == "dolphin"){
        this->setBrush(QBrush(color5));
    }
}


void BoardTransport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {

        QPixmap pixmap(boundingRect().size().toSize());
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        if (transportType_ == "boat"){
            pixmap.fill(color4);
        } else if (transportType_ == "dolphin"){
            pixmap.fill(color5);
        }
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

    }
    this->show();
}

void BoardTransport::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->acceptProposedAction();
    QStringList data = event->mimeData()->text().split(";");
    int id = data[1].toInt();
    QString type = data[0];
    QString parentType ="transport";
    qDebug() << "Got a drop to" << parentType << transportId_ << "from" << type << "id" << id;

    Student::MainWindow *win = Student::MainWindow::getInstance();
    std::unordered_map<int, std::shared_ptr<Common::Pawn>> pawnMap = boardPtr_->getPawnMap();
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > hexMap = boardPtr_->getHexMap();
    std::unordered_map<int, std::shared_ptr<Common::Transport>> transportMap = boardPtr_->getTransportMap();
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap =  win->getBoardPawnMap();
    std::unordered_map<int, Ui::BoardTransport*> boardTransportMap = win->getBoardTransportMap();

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
                    throw Common::IllegalMoveException("Transport is full") ;
                }
                // Get pawn origin coordinates from pawn map
                Common::CubeCoordinate origin;
                auto pawnIt = pawnMap.find(id);
                if (pawnIt != pawnMap.end()){
                    origin = pawnIt->second->getCoordinates();
                }

                std::vector<std::shared_ptr<Common::Pawn>> pawns = transportIt->second->getPawnsInTransport();
                for (auto x : pawns){
                    if (x == pawnIt->second){
                        throw Common::IllegalMoveException("Moving pawn inside same transport") ;
                    }
                }

                // This needs Gamestates to be implemented
                // gamePtr_->movePawn(origin, hexCoord_, pawnId);

                // This is unneccessary when upper row is executed
                boardPtr_->movePawn(id, coord);

                transportIt->second->addPawn(pawnIt->second);

                auto iter = boardPawnMap.find(id);
                if (iter != boardPawnMap.end()){
                    iter->second->setParentItem(this);
                    iter->second->setPosition(capacity, parentType);
                }

                // Get origin hex pawns and update positions
                auto hexIt = hexMap.find(origin);
                if (hexIt != hexMap.end()){
                    std::vector<std::shared_ptr<Common::Pawn>> oldPawns = hexIt->second->getPawns();
                    int i = 1;
                    for (auto x : oldPawns){
                        auto boardPawnIt = boardPawnMap.find(x->getId());
                        boardPawnIt->second->setPosition(i, "hex");
                        ++i;
                    }
                }
            }
        }
        catch (Common::IllegalMoveException& e){
            std::cout << e.msg() << std::endl;
        }
}

}
