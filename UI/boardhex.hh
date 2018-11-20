#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QtWidgets/QGraphicsPolygonItem>
#include <QtWidgets/QGraphicsSceneHoverEvent>
#include <QtGui/QFocusEvent>
#include <hex.hh>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem, std::enable_shared_from_this<BoardHex>
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    ~BoardHex() = default;
    int getSize() const;
    void drawHex(std::shared_ptr<Common::Hex>, QGraphicsScene*);
    void colorHex(std::shared_ptr<Common::Hex>);

    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);

private:
    int size_;
};
}

#endif // BOARDHEX_HH
