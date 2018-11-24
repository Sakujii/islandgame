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
#include "boardpawn.hh"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QDesktopWidget>
#include <qmath.h>
#include <iostream>

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

    std::shared_ptr<Student::GameBoard> boardPtr = std::make_shared <GameBoard>();
    std::shared_ptr<Common::IGameState> statePtr = std::make_shared<GameState>();
    std::vector<std::shared_ptr<Common::IPlayer>> playerVector;

    game_ = Common::Initialization::getGameRunner(boardPtr, statePtr, playerVector);

    initScene();

    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexMap = boardPtr->getHexMap();

    int pawnId = 0;
    for (auto x : hexMap){
        // Adding pawns to hex
        if (x.first.x < 2 && x.first.x >= -2 && x.first.z < 2 && x.first.z >= -2){
            for (int player = 1; player <= playerCount_; ++player){
                boardPtr->addPawn(player, pawnId, x.first);
                ++pawnId;
            }
        }
        std::shared_ptr<Common::Hex> hex = x.second;
         //Ui::BoardHex * boardHex = new Ui::BoardHex();

        // Shared pointer goes out from scope at the end of MainWindow constructor
        // Should we use smart pointers here or not?
        // std::shared_ptr<Ui::BoardHex> boardHex = std::make_shared<Ui::BoardHex>();

         //boardHex->drawHex(hex, boardScene, boardPtr_);
        drawHex(hex, boardPtr);

    }
}


MainWindow* MainWindow::getInstance()
{
    return mainInstance;
}

std::shared_ptr<Common::IGameRunner> MainWindow::getGame()
{
    return game_;
}

std::unordered_map<int, Ui::BoardPawn *> MainWindow::getBoardPawnMap()
{
    return boardPawnMap_;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numberOfPlayers(int count)
{
    playerCount_ = count;
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
    view->setMouseTracking(true);


}

void MainWindow::drawHex(std::shared_ptr<Common::Hex> hexPtr, std::shared_ptr<Student::GameBoard> boardPtr)
{
    Common::CubeCoordinate hexCoord = hexPtr->getCoordinates();

    Ui::BoardHex * boardHex = new Ui::BoardHex(0, hexPtr, boardPtr, game_);

    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    QPointF axial = Student::cubeToAxial(hexCoord, boardHex->getSize());

    boardScene->addItem(boardHex);
    boardHex->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    boardHex->setToolTip(QString::number(hexCoord.x) + "," + QString::number(hexCoord.z));
    boardHex->colorHex();

    std::vector<std::shared_ptr<Common::Pawn>> pawns = hexPtr->getPawns();

    int i = 1;
    for (auto x : pawns){
        Ui::BoardPawn *boardPawn = new Ui::BoardPawn(boardHex, x->getId(), x->getPlayerId());
        boardPawn->setPosition(i);
        ++i;
        if (boardPawnMap_.find(x->getId()) == boardPawnMap_.end()){
            boardPawnMap_.insert(std::make_pair(x->getId(), boardPawn));
        }
    }

}
}

