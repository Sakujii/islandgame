#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QtWidgets/QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>


namespace Ui {

class BoardPawn: public QGraphicsEllipseItem
{
public:
    explicit BoardPawn(QGraphicsItem *parent = 0);
    ~BoardPawn() = default;

    void drawPawn();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

}

#endif // BOARDPAWN_HH
