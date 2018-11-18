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

    GameBoard* boardPtr;

    initScene();
    drawHex();

}


MainWindow* MainWindow::getInstance(QWidget *parent)
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
    QWidget *sceneWidget = new QWidget(this);
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

void MainWindow::drawHex()
{
    int halfWidth = (boardScene->width())/2;
    int halfHeight = (boardScene->height()/2);

    QPolygonF polygon;
    double side = 20;
    double dx = qSqrt(3)/2 * side;
    polygon
            << QPointF(dx, -side/2)
            << QPointF(0, -side)
            << QPointF(-dx, -side/2)
            << QPointF(-dx, side/2)
            << QPointF(0, side)
            << QPointF(dx, side/2);

    Common::CubeCoordinate cube = Common::CubeCoordinate(0, 0, 0);

    QPointF axial = cubeToAxial(cube, side);

    Ui::BoardHex * hexagon = new Ui::BoardHex();
    hexagon->setPolygon(polygon);
    boardScene->addItem(hexagon);
    hexagon->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    hexagon->setToolTip(QString::number(0) + "," + QString::number(0));

    Common::CubeCoordinate cube1 = Common::CubeCoordinate (0, 0, 1);
    QPointF axial1 = cubeToAxial(cube1, side);

    Ui::BoardHex * hexagon2 = new Ui::BoardHex();
    hexagon2->setPolygon(polygon);
    boardScene->addItem(hexagon2);
    hexagon2->setPos(halfWidth + axial1.x(), halfHeight + axial1.y());
    hexagon2->setToolTip(QString::number(1) + "," + QString::number(0));

}
}

