#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QtWidgets/QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <QMimeData>
#include <memory>


namespace Ui {

class BoardPawn: public QGraphicsEllipseItem
{

public:
    explicit BoardPawn(QGraphicsItem *parent = 0, int id = 0, int playerId = 0);
    ~BoardPawn() = default;

    QRectF boundingRect() const override;
    void drawPawn();
    void setPosition(int, QString parentType);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int pawnId_;
    int playerId_;
    QBrush brush_;
};

}

#endif // BOARDPAWN_HH
