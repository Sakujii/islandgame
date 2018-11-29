#include "boardactor.hh"
#include "mainwindow.hh"

#include <QPointF>
#include <QPolygonF>
#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QMimeData>
#include <QDrag>
#include <QVector>
#include <qmath.h>


namespace Ui{

BoardActor::BoardActor(QGraphicsItem *parent, int id, std::string actorType): QGraphicsPolygonItem (parent)
{
    qDebug() << "actor created";

    this->show();

    actorId_ = id;
    actorType_ = actorType;

    size_ = 20;

    double dx = qSqrt(3)/2 * size_;
    polygon_
            << QPointF(dx, -size_/2)
            << QPointF(0, -size_)
            << QPointF(-dx, -size_/2)
            << QPointF(-dx, size_/2)
            << QPointF(0, size_)
            << QPointF(dx, size_/2);

    this->setPolygon(polygon_);

    drawActor();
    setAcceptedMouseButtons(Qt::LeftButton);

}



void BoardActor::drawActor()
{
    this->setPos(0, 0);
    this->setBrush(QBrush(Qt::black));

}

void BoardActor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {

        QPixmap pixmap(size_*2, size_*2);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setBrush(QBrush(Qt::black));
        painter.translate(size_, size_);
        painter.drawPolygon(polygon_);
        painter.end();

        QMimeData *mime = new QMimeData;
        QString data;
        data.append("actor;");
        data.append(QString::number(actorId_));
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
