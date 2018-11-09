#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneHoverEvent>
#include <QFocusEvent>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    ~BoardHex(){}

    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:

};
}

#endif // BOARDHEX_HH
