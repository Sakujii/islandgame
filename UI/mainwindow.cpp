#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"
#include "boardhex.hh"
#include "gameboard.hh"
#include "coordinateconvert.hh"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
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

    GameBoard* boardPtr = new GameBoard;

    initScene();

    for (int i = -3; i < 4; ++i ){
        Common::CubeCoordinate coord = Common::CubeCoordinate(i, 0, 0);
        std::shared_ptr<Common::Hex> newHex = std::make_shared<Common::Hex>();
        newHex->setCoordinates(coord);
        boardPtr->addHex(newHex);
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
    int width = 700;
    int height = 500;
    int border= 50;
    sceneWidget->setGeometry(border, border, width, height);

    QGraphicsView * view = new QGraphicsView(sceneWidget);
    boardScene = new QGraphicsScene(view);
    boardScene->setSceneRect(0, 0, width, height);
    view->setScene(boardScene);

}

void MainWindow::drawHex(Common::CubeCoordinate cube)
{
    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    QPointF axial = cubeToAxial(cube, 20);

    Ui::BoardHex * hexagon = new Ui::BoardHex();
    boardScene->addItem(hexagon);
    hexagon->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    hexagon->setToolTip(QString::number(cube.x) + "," + QString::number(cube.z));

}
}

