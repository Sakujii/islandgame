#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QtWidgets/QGraphicsPolygonItem>
#include <hex.hh>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem, public std::enable_shared_from_this<BoardHex>
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    int getSize() const;
    void drawHex(std::shared_ptr<Common::Hex>, QGraphicsScene*);
    void colorHex();

    void mousePressEvent(QGraphicsSceneMouseEvent*);

private:
    int size_;
    Common::CubeCoordinate hexCoord_;
    std::shared_ptr<Common::Hex> hexPtr_;
};
}

#endif // BOARDHEX_HH
