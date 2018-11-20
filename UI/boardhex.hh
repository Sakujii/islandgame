#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneHoverEvent>
#include <QFocusEvent>
#include <hex.hh>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    ~BoardHex() = default;
    int getSize() const;
    void drawHex(std::shared_ptr<Common::Hex>, QGraphicsScene*);
    void colorHex(std::shared_ptr<Common::Hex>);

    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:
    int size_;
};
}

#endif // BOARDHEX_HH
