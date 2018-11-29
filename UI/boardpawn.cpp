#include "boardpawn.hh"
#include "mainwindow.hh"
#include "boardhex.hh"

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QDebug>


namespace Ui{

BoardPawn::BoardPawn(QGraphicsItem* parent, int id, int playerId): QGraphicsEllipseItem (parent)
{
    pawnId_ = id;
    playerId_ = playerId;

    if (playerId_ == 1){
        brush_ = QBrush(Qt::black);
    } else if (playerId_ == 2){
        brush_ = (QBrush(Qt::red));
    } else if (playerId_ == 3){
        brush_ = (QBrush(Qt::darkGreen));
    }

    drawPawn();
    // Draw pawns on top of actors
    this->setZValue(1);
    setAcceptedMouseButtons(Qt::LeftButton);

}

void BoardPawn::drawPawn()
{
    this->setRect(boundingRect());
    this->setBrush(brush_);
}

void BoardPawn::setPosition(int pawnAmount, QString parentType)
{
    QPoint hex1 = QPoint(-7, -22);
    QPoint hex2 = QPoint(-20, -14);
    QPoint hex3 = QPoint(7, -14);

    QPoint transport1 = QPoint(0, -7);
    QPoint transport2 = QPoint(10, -7);
    QPoint transport3 = QPoint(20, -7);


    if (parentType == "hex"){
        if (pawnAmount == 1){
            this->setPos(hex1);
        } else if (pawnAmount == 2){
            this->setPos(hex2);
        } else if (pawnAmount == 3){
            this->setPos(hex3);
        }
    }
    else {
        if (pawnAmount == 3){
            this->setPos(transport1);
        } else if (pawnAmount == 2){
            this->setPos(transport2);
        } else if (pawnAmount == 1){
            this->setPos(transport3);
        }
    }
}

void BoardPawn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {

        QPixmap pixmap(boundingRect().size().toSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setBrush(brush_);
        painter.drawEllipse(boundingRect());
        painter.end();

        QMimeData *mime = new QMimeData;
        QString data;
        data.append("pawn;");
        data.append(QString::number(pawnId_));
        mime->setText(data);
        QDrag *drag = new QDrag(win);
        drag->setMimeData(mime);
        drag->setHotSpot(QPoint(5, 7));
        drag->setPixmap(pixmap);
        drag->exec();

    }
this->show();
}


QRectF BoardPawn::boundingRect() const
{
    int size = 14;
    return QRectF(0, 0, size, size);
}

}
