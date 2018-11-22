#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QtWidgets/QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <memory>


namespace Ui {

class BoardPawn: public QGraphicsEllipseItem
{

public:
    explicit BoardPawn(QGraphicsItem *parent = 0);
    ~BoardPawn() = default;

    QRectF boundingRect() const override;
    void drawPawn();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
};

}

#endif // BOARDPAWN_HH
