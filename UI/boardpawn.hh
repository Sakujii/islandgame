#ifndef BOARDPAWN_HH
#define BOARDPAWN_HH

#include <QtWidgets/QGraphicsEllipseItem>


namespace Ui {

class BoardPawn: public QGraphicsEllipseItem, public std::enable_shared_from_this<BoardPawn>
{
public:
    explicit BoardPawn(QGraphicsItem *parent = 0);
    ~BoardPawn() = default;

    void drawPawn();
};

}

#endif // BOARDPAWN_HH
