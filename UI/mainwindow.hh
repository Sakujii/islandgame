#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"
#include "boardpawn.hh"
#include "boardtransport.hh"
#include "boardhex.hh"
#include "boardpawn.hh"
#include "gamestate.hh"
#include "boardactor.hh"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>

namespace Ui {
class MainWindow;
}

namespace Student{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow* getInstance();
    std::shared_ptr<Common::IGameRunner> getGame();
    std::map<Common::CubeCoordinate, Ui::BoardHex*> getBoardHexMap();
    std::unordered_map<int, Ui::BoardPawn*> getBoardPawnMap();
    std::unordered_map<int, Ui::BoardTransport*> getBoardTransportMap();
    std::unordered_map<int, Ui::BoardActor*> getBoardActorMap();
    void addBoardTransport(std::shared_ptr<Common::Hex>,
                           Ui::BoardHex*,
                           std::shared_ptr<Student::GameBoard>);
    void addBoardActor(std::shared_ptr<Common::Hex>,
                       Ui::BoardHex*);

    void drawHex(std::shared_ptr<Common::Hex>, std::shared_ptr<Student::GameBoard>);
    void removeBoardPawn(int);
    void removeBoardActor(int);
    void removeBoardTransport(int);
    void numberOfGamephase();
    void numberOfCurrentPlayer(int playerid);
    void numberOfActionsLeft(int actionsleft);
    void updatePointsList();
    void setGameMessage(std::string);
    void spinWheelMovie();

public slots:
    void numberOfPlayers(int count);
    void nextGamephase();
    void spinWheel();
    void stopWheelMovie();

private:
    Ui::MainWindow *ui;
    static MainWindow *mainInstance;
    QGraphicsScene *boardScene;
    std::shared_ptr<Common::IGameRunner> game_;
    std::map<Common::CubeCoordinate, Ui::BoardHex*> boardHexMap_;
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap_;
    std::unordered_map<int, Ui::BoardTransport*> boardTransportMap_;
    std::unordered_map<int, Ui::BoardActor*> boardActorMap_;
    void initScene();
    int playerCount_;
    std::shared_ptr<Student::GameState> state_;
    QMovie *movie_;

};
}


#endif // MAINWINDOW_HH
