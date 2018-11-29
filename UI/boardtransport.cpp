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
                               std::shared_ptr<Student::GameBoard> boardPtr): QGraphicsRectItem(parent)
{
    boardPtr_ = boardPtr;
    transportId_ = id;
    transportType_ = transportType;
    if (transportType_ == "boat"){
        brush_ = QBrush(QColor(110, 30, 5)); // Brown for boat
    } else if (transportType_ == "dolphin"){
        brush_ = QBrush(QColor(Qt::gray));
    }

    drawTransport();
    setAcceptedMouseButtons(Qt::LeftButton);
    this->setPos(-18, 3);
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
