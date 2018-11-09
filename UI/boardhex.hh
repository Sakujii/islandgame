#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <QGraphicsPolygonItem>

namespace Ui{

class BoardHex : public QGraphicsPolygonItem
{

public:
    BoardHex(QGraphicsItem *parent = 0);
    ~BoardHex(){}

private:
};
}

#endif // BOARDHEX_HH
