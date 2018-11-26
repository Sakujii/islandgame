#ifndef BOARDTRANSPORT_HH
#define BOARDTRANSPORT_HH

#include <QtWidgets/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <QMimeData>
#include <memory>

namespace Ui {

class BoardTransport: public QGraphicsRectItem
{

public:
    explicit BoardTransport(QGraphicsItem *parent = 0, int id = 0, std::string transportType = "");
    ~BoardTransport() = default;

    QRectF boundingRect() const override;
    void drawTransport();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int transportId_;
    std::string transportType_;
};

}

#endif // BOARDTRANSPORT_HH