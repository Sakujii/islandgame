#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"
#include "boardhex.hh"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <qmath.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Open dialog and connect the number of players to mainwindow
    Dialog dialog;
    connect(&dialog, &Dialog::accept, this, &MainWindow::numberOfPlayers);
    dialog.setModal(true);
    dialog.exec();

    drawboard();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numberOfPlayers(int count)
{
    ui->labelNumber->setText(QString::number(count));
}

void MainWindow::drawboard()
{
    QWidget *sceneWidget = new QWidget(this);
    sceneWidget->show();
    sceneWidget->setGeometry(50, 50, 700, 500);

    QGraphicsView * view = new QGraphicsView(sceneWidget);
    QGraphicsScene * scene = new QGraphicsScene(view);
    view->setScene(scene);
    scene->setSceneRect(sceneWidget->x(), sceneWidget->y(), sceneWidget->width(), sceneWidget->height());

    QPolygonF polygon;
    double side = 25;
    double dx = qSqrt(3)/2 * side;
    polygon
            << QPointF(dx, -side/2)
            << QPointF(0, -side)
            << QPointF(-dx, -side/2)
            << QPointF(-dx, side/2)
            << QPointF(0, side)
            << QPointF(dx, side/2);

    Ui::BoardHex * hexagon = new Ui::BoardHex();
    hexagon->setPolygon(polygon);
    scene->addItem(hexagon);
    hexagon->setPos(dx * 5, side * 1.5 * 5);
    hexagon->setToolTip(QString::number(0) + "," + QString::number(0));

    Ui::BoardHex * hexagon2 = new Ui::BoardHex();
    hexagon2->setPolygon(polygon);
    scene->addItem(hexagon2);
    hexagon2->setPos(dx * 7, side * 1.5 * 5);
    hexagon2->setToolTip(QString::number(6) + "," + QString::number(5));

    Ui::BoardHex * hexagon3 = new Ui::BoardHex();
    hexagon3->setPolygon(polygon);
    scene->addItem(hexagon3);
    hexagon3->setPos(dx * 6, side * 1.5 * 6);
    hexagon3->setToolTip(QString::number(5) + "," + QString::number(6));

}

