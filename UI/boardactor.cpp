#include "boardactor.hh"
#include "mainwindow.hh"

#include <QPointF>
#include <QPolygonF>
#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QMimeData>
#include <QDrag>
#include <qmath.h>


namespace Ui{

BoardActor::BoardActor(QGraphicsItem *parent, int id, std::string actorType): QGraphicsPolygonItem (parent)
{
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

    if (actorType_ == "vortex"){
        brush_ = QBrush(Qt::black);
    } else if (actorType_ == "kraken"){
        brush_ = (QBrush(Qt::red));
    } else if (actorType_ == "seamunster"){
        brush_ = (QBrush(Qt::darkGreen));
    } else if (actorType_ == "shark"){
        brush_ = (QBrush(Qt::darkGray));
    }

    drawActor();
    setAcceptedMouseButtons(Qt::LeftButton);

}



void BoardActor::drawActor()
{
    this->setPos(0, 0);
    QPointF textPos = QPointF(-10, -18);
    QFont font = QFont("Colibri", 30);
    QColor textColor = QColor(Qt::white);

    if (actorType_ == "vortex"){
        this->setBrush(brush_);
        QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem("V", this);
        textItem->setBrush(textColor);
        textItem->setFont(font);
        textItem->setPos(textPos);
    } else if (actorType_ == "kraken"){
        this->setBrush(brush_);
        QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem("K", this);
        textItem->setBrush(textColor);
        textItem->setFont(font);
        textItem->setPos(textPos);
    } else if (actorType_ == "seamunster"){
        this->setBrush(brush_);
        QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem("M", this);
        textItem->setBrush(textColor);
        textItem->setFont(font);
        textItem->setPos(textPos);
    } else if (actorType_ == "shark"){
        this->setBrush(brush_);
        QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem("S", this);
        textItem->setBrush(textColor);
        textItem->setFont(font);
        textItem->setPos(textPos);
    }


}

void BoardActor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {

        QPixmap pixmap(size_*2, size_*2);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setBrush(brush_);
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
        drag->setHotSpot(QPoint(30, 30));
        drag->setPixmap(pixmap);
        drag->exec();

    }
    this->show();
}

}
