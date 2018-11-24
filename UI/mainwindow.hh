#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"
#include "boardpawn.hh"

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
    std::unordered_map<int, Ui::BoardPawn*> getBoardPawnMap();

    void drawHex(std::shared_ptr<Common::Hex>, std::shared_ptr<Student::GameBoard>);

public slots:
    void numberOfPlayers(int count);

private:
    Ui::MainWindow *ui;
    static MainWindow *mainInstance;
    QGraphicsScene *boardScene;
    std::shared_ptr<Common::IGameRunner> game_;
    std::unordered_map<int, Ui::BoardPawn*> boardPawnMap_;
    void initScene();
    int playerCount_;

};
}


#endif // MAINWINDOW_HH
