#ifndef BOARDACTOR_HH
#define BOARDACTOR_HH


#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMimeData>

namespace Ui {

class BoardActor: public QGraphicsPolygonItem
{

public:
    explicit BoardActor(QGraphicsItem *parent = 0,
                            int id = 0,
                            std::string actorType = "");

    ~BoardActor() = default;

    void drawActor();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int size_;
    int actorId_;
    QBrush brush_;
    std::string actorType_;
    QPolygonF polygon_;
    QGraphicsSimpleTextItem *textItem_;
};

}

#endif // BOARDACTOR_HH
