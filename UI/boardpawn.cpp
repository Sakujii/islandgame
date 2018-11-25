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

QColor color1 = Qt::black;
QColor color2 = Qt::darkGreen;
QColor color3 = Qt::red;

BoardPawn::BoardPawn(QGraphicsItem* parent, int id, int playerId): QGraphicsEllipseItem (parent)
{
    pawnId_ = id;
    playerId_ = playerId;
    drawPawn();
    setAcceptedMouseButtons(Qt::LeftButton);

}

void BoardPawn::drawPawn()
{
    this->setRect(boundingRect());

    if (playerId_ == 1){
        this->setBrush(QBrush(color1));
    } else if (playerId_ == 2){
        this->setBrush(QBrush(color2));
    } else if (playerId_ == 3){
        this->setBrush(QBrush(color3));
    }
}

void BoardPawn::setPosition(int pawnAmount)
{
    if (pawnAmount == 1){
        this->setPos(-5, -15);
    } else if (pawnAmount == 2){
        this->setPos(-13, -5);
    } else if (pawnAmount == 3){
        this->setPos(3, -5);
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
        if (playerId_ == 1){
            painter.setBrush(QBrush(color1));
        } else if (playerId_ == 2){
            painter.setBrush(QBrush(color2));
        } else if (playerId_ == 3){
            painter.setBrush(QBrush(color3));
        }
        painter.drawEllipse(boundingRect());
        painter.end();

        QMimeData *mime = new QMimeData;
        mime->setText(QString::number(pawnId_));
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
    int size = 10;
    return QRectF(0, 0, size, size);
}

}
