#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <hex.hh>

#include <QtWidgets/QGraphicsPolygonItem>
#include <memory>


namespace Ui{

class BoardHex : public QGraphicsPolygonItem
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0);
    int getSize() const;
    void drawHex(std::shared_ptr<Common::Hex>, QGraphicsScene*);
    void colorHex();

    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    int size_;
    Common::CubeCoordinate hexCoord_;
    std::shared_ptr<Common::Hex> hexPtr_;
};
}

#endif // BOARDHEX_HH
