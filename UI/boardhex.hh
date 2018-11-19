#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtWidgets/QGraphicsSceneHoverEvent>
#include <QtGui/QFocusEvent>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    ~BoardHex() = default;
    int getSize() const;

    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:
    int size_;
};
}

#endif // BOARDHEX_HH
