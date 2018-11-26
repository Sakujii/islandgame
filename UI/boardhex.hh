#ifndef BOARDHEX_HH
#define BOARDHEX_HH

#include <hex.hh>
#include <gameboard.hh>
#include <boardpawn.hh>
#include <igamerunner.hh>

#include <QtWidgets/QGraphicsPolygonItem>
#include <memory>
#include <map>
#include <unordered_map>


namespace Ui{

class BoardHex : public QGraphicsPolygonItem, public std::enable_shared_from_this<BoardHex>
{

public:
    explicit BoardHex(QGraphicsItem *parent = 0,
                      std::shared_ptr<Common::Hex> = nullptr,
                      std::shared_ptr<Student::GameBoard> = nullptr,
                      std::shared_ptr<Common::IGameRunner> = nullptr);
    int getSize() const;
    void drawHex(std::shared_ptr<Common::Hex>, QGraphicsScene*, std::shared_ptr<Student::GameBoard>);
    void colorHex();
    void addActors();
    void addTransports();
    void removePawns();

    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    int size_;
    Common::CubeCoordinate hexCoord_;
    std::shared_ptr<Common::Hex> hexPtr_;
    std::shared_ptr<Student::GameBoard> boardPtr_;
    std::shared_ptr<Common::IGameRunner> gamePtr_;
};
}

#endif // BOARDHEX_HH
