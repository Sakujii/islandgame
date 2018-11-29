#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"
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
#include <iostream>
#include <memory>

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
    std::shared_ptr<Student::GameState> statePtr = std::make_shared <GameState>();
    state_=statePtr;
    std::vector<std::shared_ptr<Common::IPlayer>> playerVector;
    for (int i = 0; i < playerCount_; ++i){
        std::shared_ptr<Common::IPlayer> player = std::make_shared<Player>(i+1);
        playerVector.push_back(player);
    }

    game_ = Common::Initialization::getGameRunner(boardPtr, statePtr, playerVector);

    initScene();
    numberOfGamephase(statePtr->currentGamePhase());
    numberOfCurrentPlayer(statePtr->currentPlayer());
    connect(ui->pushButtonGamephase, &QPushButton::clicked, this, &MainWindow::nextGamephase);
    connect(ui->pushButtonSpinWheel, &QPushButton::clicked, this, &MainWindow::spinWheel);

    state_->initPoints(playerCount_);
//    std::vector<std::string> v = {"7", "5", "16", "8"};
//    std::string s = "test3";
//    ui->listWidgetPoints->addItem("Test");
//    ui->listWidgetPoints->addItem("Test2");
//    ui->listWidgetPoints->addItem(QString::fromStdString(s));
//    mainInstance->updatePointsList();


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
        drawHex(hex, boardPtr);

    }
}

MainWindow::~MainWindow()
{
    delete ui;
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

std::unordered_map<int, Ui::BoardTransport *> MainWindow::getBoardTransportMap()
{
    return boardTransportMap_;
}

std::unordered_map<int, Ui::BoardActor *> MainWindow::getBoardActorMap()
{
    return boardActorMap_;
}

void MainWindow::addBoardTransport(std::shared_ptr<Common::Hex> hexPtr,
                                   Ui::BoardHex* boardHex,
                                   std::shared_ptr<Student::GameBoard> boardPtr)
{
    std::vector<std::shared_ptr<Common::Transport>> transports = hexPtr->getTransports();

    for (auto x : transports){
        Ui::BoardTransport *boardTransport =
                new Ui::BoardTransport(boardHex, x->getId(), x->getTransportType(), boardPtr);
        if (boardTransportMap_.find(x->getId()) == boardTransportMap_.end()){
            boardTransportMap_.insert(std::make_pair(x->getId(), boardTransport));
        }
    }
}

void MainWindow::addBoardActor(std::shared_ptr<Common::Hex> hexPtr,
                               Ui::BoardHex *boardHex)
{
    std::vector<std::shared_ptr<Common::Actor>> actors = hexPtr->getActors();

    for (auto x : actors){
        Ui::BoardActor *boardActor =
                new Ui::BoardActor(boardHex, x->getId(), x->getActorType());
        if (boardActorMap_.find(x->getId()) == boardActorMap_.end());
            boardActorMap_.insert(std::make_pair(x->getId(), boardActor));
    }

}


void MainWindow::numberOfPlayers(int count)
{
    playerCount_ = count;
    ui->labelNumber->setText(QString::number(count));
}

void MainWindow::initScene()
{
    QWidget *sceneWidget = new QWidget(this);
    sceneWidget->show();
    int mainWidth = 1300;
    int mainHeight = 900;
    int xborder = 300;
    int yborder= 50;
    int sceneWidth = 1500;
    int sceneHeight = 1200;
    sceneWidget->setGeometry(xborder, yborder, mainWidth-1.5*xborder, mainHeight- 4*yborder);
    this->resize(mainWidth, mainHeight);

    QGraphicsView * view = new QGraphicsView(sceneWidget);
    boardScene = new QGraphicsScene(view);
    view->resize(sceneWidget->width(), sceneWidget->height());
    boardScene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    view->setScene(boardScene);


}

void MainWindow::drawHex(std::shared_ptr<Common::Hex> hexPtr, std::shared_ptr<Student::GameBoard> boardPtr)
{
    Common::CubeCoordinate hexCoord = hexPtr->getCoordinates();

    Ui::BoardHex *boardHex = new Ui::BoardHex(0, hexPtr, boardPtr, game_);

    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    QPointF axial = Student::cubeToAxial(hexCoord, boardHex->getSize());

    boardScene->addItem(boardHex);
    boardHex->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    boardHex->setToolTip(QString::number(hexCoord.x) + "," + QString::number(hexCoord.z));
    boardHex->colorHex();

    std::vector<std::shared_ptr<Common::Pawn>> pawns = hexPtr->getPawns();
    QString parentType = "hex";

    int i = 1;
    for (auto x : pawns){
        Ui::BoardPawn *boardPawn = new Ui::BoardPawn(boardHex, x->getId(), x->getPlayerId());
        boardPawn->setPosition(i, parentType);
        ++i;
        if (boardPawnMap_.find(x->getId()) == boardPawnMap_.end()){
            boardPawnMap_.insert(std::make_pair(x->getId(), boardPawn));
        }
    }
    addBoardTransport(hexPtr, boardHex, boardPtr);


}

void MainWindow::removeBoardPawn(int id)
{
    auto it = boardPawnMap_.find(id);
    if (it != boardPawnMap_.end()){
        boardScene->removeItem(it->second);
        boardPawnMap_.erase(it);
    }
}

void MainWindow::numberOfGamephase(int phaseid)
{
    ui->labelGamePhaseNumber->setText(QString::number(phaseid));
}

void MainWindow::numberOfCurrentPlayer(int playerid)
{
    ui->labelCurrentPlayerNumber->setText(QString::number(playerid));
}

void MainWindow::numberOfActionsLeft(int actionsleft)
{
    ui->labelActionsleftNumber->setText(QString::number(actionsleft));
}

void MainWindow::nextGamephase()
{
    Common::GamePhase phase = state_->currentGamePhase();
    if(phase==Common::MOVEMENT){state_->changeGamePhase(Common::SINKING);}
    else if(phase==Common::SINKING){state_->changeGamePhase(Common::SPINNING);}
    else
    {
        state_->changeGamePhase(Common::MOVEMENT);
        game_->getCurrentPlayer()->setActionsLeft(3);
        numberOfActionsLeft(3);
        int currentplayerid = state_->currentPlayer();
        if(currentplayerid==playerCount_)
        {
            state_->changePlayerTurn(1);
        }
        else
        {
            state_->changePlayerTurn(currentplayerid+1);
        }
        numberOfCurrentPlayer(state_->currentPlayer());
    }

    ui->labelGamePhaseNumber->setText(QString::number(state_->currentGamePhase()));
    mainInstance->updatePointsList();
    state_->addPointsToPlayer(1,2);
}

void MainWindow::spinWheel()
{
    std::pair<std::string,std::string> wheelresult = game_->spinWheel();
    std::string animal = wheelresult.first;
    std::string amount = wheelresult.second;
    ui->labelWhatMovesId->setText(QString::fromStdString(animal));
    ui->labelMoveAmountNumber->setText(QString::fromStdString(amount));
}

void MainWindow::updatePointsList()
{
    ui->listWidgetPoints->clear();
    std::vector<std::string> pointslist = state_->getPointsVectorForPlot();
    for(auto s : pointslist)
        ui->listWidgetPoints->addItem(QString::fromStdString(s));
}

}

