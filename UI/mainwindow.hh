#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "cubecoordinate.hh"
#include "hex.hh"
#include "igamerunner.hh"
#include "gameboard.hh"

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

    void drawHex(std::shared_ptr<Common::Hex> hexPtr);

public slots:
    void numberOfPlayers(int count);

private:
    Ui::MainWindow *ui;
    static MainWindow *mainInstance;
    QGraphicsScene *boardScene;
    std::shared_ptr<Common::IGameRunner> game_;
    void initScene();
    int playerCount_;

};
}


#endif // MAINWINDOW_HH
