#ifndef BOARDTRANSPORT_HH
#define BOARDTRANSPORT_HH

#include "gameboard.hh"
#include "igamerunner.hh"

#include <QtWidgets/QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsObject>
#include <QPainter>
#include <QMimeData>
#include <memory>

namespace Ui {

class BoardTransport: public QGraphicsRectItem
{

public:
    explicit BoardTransport(QGraphicsItem *parent = 0,
                            int id = 0,
                            std::string transportType = "",
                            std::shared_ptr<Student::GameBoard> boardPtr = nullptr,
                            std::shared_ptr<Common::IGameRunner> = nullptr);

    ~BoardTransport() = default;

    QRectF boundingRect() const override;
    void drawTransport();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;


private:
    int transportId_;
    std::string transportType_;
    std::shared_ptr<Student::GameBoard> boardPtr_;
    std::shared_ptr<Common::IGameRunner> gamePtr_;
    QBrush brush_;
};

}

#endif // BOARDTRANSPORT_HH
