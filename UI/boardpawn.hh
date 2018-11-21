#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QtWidgets/QGraphicsEllipseItem>


namespace Ui {

class BoardPawn: public QGraphicsEllipseItem
{
public:
    explicit BoardPawn(QGraphicsItem *parent = 0);
    ~BoardPawn() = default;
};

}

#endif // BOARDPAWN_HH
