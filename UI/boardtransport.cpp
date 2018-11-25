#include "boardtransport.hh"
#include "mainwindow.hh"

#include <QGraphicsScene>
#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QDebug>
#include <iostream>

namespace Ui {

QColor color4 = QColor(110, 30, 5);
QColor color5 = Qt::gray;

BoardTransport::BoardTransport(QGraphicsItem *parent, int id, std::string transportType): QGraphicsRectItem(parent)
{
    transportId_ = id;
    transportType_ = transportType;
    drawTransport();
    setAcceptedMouseButtons(Qt::LeftButton);
    this->setPos(-13, 3);

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

}
