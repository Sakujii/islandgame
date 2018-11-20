#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"
#include "boardhex.hh"
#include "gameboard.hh"
#include "coordinateconvert.hh"
#include "gamestate.hh"
#include "player.hh"
#include "igamerunner.hh"
#include "initialize.hh"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QDesktopWidget>
#include <qmath.h>

namespace Student{

MainWindow* MainWindow::mainInstance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainInstance = this;

    // Open dialog and connect the number of players to mainwindow
    Dialog dialog;
    connect(&dialog, &Dialog::acceptSignal, this, &MainWindow::numberOfPlayers);
    dialog.setModal(true);
    dialog.exec();

    std::shared_ptr<GameBoard> boardPtr = std::make_shared <GameBoard>();
    std::shared_ptr<Common::IGameState> statePtr = std::make_shared<GameState>();
    std::vector<std::shared_ptr<Common::IPlayer>> playerVector;

    std::shared_ptr<Common::IGameRunner> game_ =
            Common::Initialization::getGameRunner(boardPtr, statePtr, playerVector);

    initScene();

    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexMap = boardPtr->getHexMap();

    for (auto x : hexMap){
        std::shared_ptr<Common::Hex> hex = x.second;
        Ui::BoardHex * boardHex = new Ui::BoardHex();
        // Why this shared_ptr does not work?
        //std::shared_ptr<Ui::BoardHex> boardHex = std::make_shared<Ui::BoardHex>();
        boardHex->drawHex(hex, boardScene);
    }
}


MainWindow* MainWindow::getInstance()
{
    return mainInstance;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numberOfPlayers(int count)
{
    ui->labelNumber->setText(QString::number(count));
}

void MainWindow::initScene()
{
    QWidget *sceneWidget = new QWidget(ui->centralwidget);
    sceneWidget->show();
    int width = 800;
    int height = 600;
    int border= 50;
    sceneWidget->setGeometry(border, border, width, height);
    this->resize(width + 2*border, height+ 2*border);

    QGraphicsView * view = new QGraphicsView(sceneWidget);
    boardScene = new QGraphicsScene(view);
    view->setFixedSize(width, height);
    boardScene->setSceneRect(0, 0, width-border, height-border);
    view->setScene(boardScene);

}

void MainWindow::drawHex(std::shared_ptr<Common::Hex> hexPtr)
{
    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    Common::CubeCoordinate cube = hexPtr->getCoordinates();

    QPointF axial = cubeToAxial(cube, 15);

    Ui::BoardHex * hexagon = new Ui::BoardHex();
    boardScene->addItem(hexagon);
    hexagon->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    hexagon->setToolTip(QString::number(cube.x) + "," + QString::number(cube.z));

}
}

