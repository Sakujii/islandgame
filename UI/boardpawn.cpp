#include "boardpawn.hh"
#include "mainwindow.hh"

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QDebug>



namespace Ui{


BoardPawn::BoardPawn(QGraphicsItem* parent): QGraphicsEllipseItem (parent)
{
    drawPawn();
    setAcceptedMouseButtons(Qt::LeftButton);

}

void BoardPawn::drawPawn()
{
    this->setRect(boundingRect());
    this->setBrush(QBrush(Qt::black));
}

void BoardPawn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->hide();
    Student::MainWindow *win = Student::MainWindow::getInstance();

    if (event->button() == Qt::LeftButton) {
        QMimeData *mime = new QMimeData;
        QDrag *drag = new QDrag(win);
        drag->setMimeData(mime);

        QPixmap pixmap(boundingRect().size().toSize());
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(Qt::black));
        painter.drawEllipse(boundingRect());
        painter.end();

        drag->setHotSpot(QPoint(5, 7));

        drag->setPixmap(pixmap);

        drag->exec();
    }
this->show();
}


QRectF BoardPawn::boundingRect() const
{
    return QRectF(0, 0, 10, 10);
}

}
