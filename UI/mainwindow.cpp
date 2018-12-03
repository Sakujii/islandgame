#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"
#include "gameboard.hh"
#include "coordinateconvert.hh"
#include "gamestate.hh"
#include "player.hh"
#include "igamerunner.hh"
#include "initialize.hh"
#include "ioexception.hh"

#include <QGraphicsView>
#include <QHBoxLayout>
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

    // Create shared pointers needed for the game
    std::shared_ptr<Student::GameBoard> boardPtr = std::make_shared <GameBoard>();
    std::shared_ptr<Student::GameState> statePtr = std::make_shared <GameState>();
    state_ = statePtr;
    std::vector<std::shared_ptr<Common::IPlayer>> playerVector;
    for (int i = 0; i < playerCount_; ++i){
        std::shared_ptr<Common::IPlayer> player = std::make_shared<Player>(i+1);
        playerVector.push_back(player);
    }
    // Call gamerunner initialization
    game_ = Common::Initialization::getGameRunner(boardPtr, statePtr, playerVector);

    // Connect ui buttons to slots
    connect(ui->pushButtonGamephase, &QPushButton::clicked, this, &MainWindow::nextGamephase);
    connect(ui->pushButtonSpinWheel, &QPushButton::clicked, this, &MainWindow::spinWheel);

    // Initialize ui objects
    initScene();
    numberOfGamephase();
    numberOfCurrentPlayer(statePtr->currentPlayer());
    numberOfActionsLeft(game_->getCurrentPlayer()->getActionsLeft());
    state_->initPoints(playerCount_);

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
        // Adding hex graphics
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

std::shared_ptr<Student::GameState> MainWindow::getState()
{
    return state_;
}

std::map<Common::CubeCoordinate, Ui::BoardHex *> MainWindow::getBoardHexMap()
{
    return boardHexMap_;
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
                new Ui::BoardTransport(boardHex, x->getId(), x->getTransportType(), boardPtr, game_);
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
        if (boardActorMap_.find(x->getId()) == boardActorMap_.end()){
            boardActorMap_.insert(std::make_pair(x->getId(), boardActor));
        }
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
    int xborder = 400;
    int yborder= 50;
    int sceneWidth = 1500;
    int sceneHeight = 1200;
    sceneWidget->setGeometry(xborder, yborder, mainWidth-1.5*xborder, mainHeight- 5*yborder);
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

    // Create new graphical hex and insert it to it's map
    Ui::BoardHex *boardHex = new Ui::BoardHex(0, hexPtr, boardPtr, game_);
    if(boardHexMap_.find(hexCoord) == boardHexMap_.end()){
        boardHexMap_.insert(std::make_pair(hexCoord, boardHex));
    }

    double halfWidth = (boardScene->width())/2;
    double halfHeight = (boardScene->height()/2);

    // Convert hex coordinates to axial and draw move graphics to it's spot
    QPointF axial = Student::cubeToAxial(hexCoord, boardHex->getSize());
    boardScene->addItem(boardHex);
    boardHex->setPos(halfWidth + axial.x(), halfHeight + axial.y());
    boardHex->setToolTip(QString::number(hexCoord.x) + "," + QString::number(hexCoord.z));
    boardHex->colorHex();

    // If there are pawns on the hex, add pawn graphics on top of the hex
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
    // If there is transport on the hex, add transport graphics
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

void MainWindow::removeBoardActor(int id)
{
    auto it = boardActorMap_.find(id);
    if (it != boardActorMap_.end()){
        boardScene->removeItem(it->second);
        boardActorMap_.erase(it);
    }
}

void MainWindow::removeBoardTransport(int id)
{
    auto it = boardTransportMap_.find(id);
    if (it != boardTransportMap_.end()){
        boardScene->removeItem(it->second);
        boardTransportMap_.erase(it);
    }
}

void MainWindow::numberOfGamephase()
{
     ui->labelGamePhaseNumber->setText(QString::fromStdString(state_->currentGamePhaseString()));
}

void MainWindow::numberOfCurrentPlayer(int playerid)
{
    QString player = QString::number(playerid);
    if (playerid == 1){
        player.append(" : black");
    } else if (playerid == 2){
        player.append(" : red");
    } else if (playerid == 3){
        player.append(" : green");
    } else{}

    ui->labelCurrentPlayerNumber->setText(player);
}

void MainWindow::numberOfActionsLeft(int actionsleft)
{
    ui->labelActionsleftNumber->setText(QString::number(actionsleft));
}

void MainWindow::nextGamephase()
{
    Common::GamePhase phase = state_->currentGamePhase();
    if (phase==Common::MOVEMENT) {state_->changeGamePhase(Common::SINKING);}
    else if (phase==Common::SINKING) {state_->changeGamePhase(Common::SPINNING);}
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

    ui->labelGamePhaseNumber->setText(QString::fromStdString(state_->currentGamePhaseString()));
    mainInstance->updatePointsList();
    state_->setSpinResult(std::make_pair("nada", "0"));
    state_->setSpinsLeft(1);
}

void MainWindow::spinWheel()
{
    if(state_->getSpinsLeft() > 0){
        spinWheelMovie();

        std::string animal;
        std::string amount;

        std::pair<std::string,std::string> wheelresult = game_->spinWheel();
        state_->setSpinResult(wheelresult);
        state_->setSpinsLeft(0);
        animal = wheelresult.first;
        amount = wheelresult.second;
        ui->labelWhatMovesId->setText(QString::fromStdString(animal));
        ui->labelMoveAmountNumber->setText(QString::fromStdString(amount));

        // If conditions to stop the movie at the right frame
        int endFrame = 0;
        if (amount == "dive"){
            if (animal == "kraken"){
                endFrame = 0;
            } else if (animal == "shark") {
                endFrame = 1;
            } else if (animal == "seamunster"){
                endFrame = 2;
            } else if (animal == "dolphin"){
                endFrame = 3;
            } else {}
        } else if (amount == "3"){
            if (animal == "kraken"){
                endFrame = 4;
            } else if (animal == "shark") {
                endFrame = 5;
            } else if (animal == "seamunster"){
                endFrame = 6;
            } else if (animal == "dolphin"){
                endFrame = 7;
            } else {}
        } else if (amount == "2"){
            if (animal == "kraken"){
                endFrame = 8;
            } else if (animal == "shark") {
                endFrame = 9;
            } else if (animal == "seamunster"){
                endFrame = 10;
            } else if (animal == "dolphin"){
                endFrame = 11;
            } else {}
        } else if  (amount == "1"){
            if (animal == "kraken"){
                endFrame = 12;
            } else if (animal == "shark") {
                endFrame = 13;
            } else if (animal == "seamunster"){
                endFrame = 14;
            } else if (animal == "dolphin"){
                endFrame = 15;
            } else {}
        } else {}

        // Connect movie to stop when at the right frame
        connect(movie_, &QMovie::frameChanged, [=](int frameNumber){
            if(frameNumber == endFrame){
                movie_->stop();
            } else{}
        });

    } else {
        setGameMessage("You are not allowed to spin!");
    }


}

void MainWindow::updatePointsList()
{
    ui->listWidgetPoints->clear();
    std::vector<std::string> pointslist = state_->getPointsVectorForPlot();
    for(auto s : pointslist){
        ui->listWidgetPoints->addItem(QString::fromStdString(s));
    }
    if(boardPawnMap_.size()==0)
    {
        setGameMessage(state_->getWinner());
    }
}

void MainWindow::setGameMessage(std::string msg)
{
    QString message = QString::fromStdString(msg);
    ui->labelGameMessageText->setText(message);
}

void MainWindow::spinWheelMovie()
{
    try{
        movie_ = new QMovie(":/wheel.gif");

        if (!movie_->isValid()) {
            throw Common::IoException
                    ("There was a problem reading the movie file");
        }

        wheelLabel_ = new QLabel(this);
        wheelLabel_->setGeometry(40, 370, 320, 350);
        wheelLabel_->setMovie(movie_);
        movie_->start();
        wheelLabel_->show();
    }
    catch (Common::IoException& e){
        setGameMessage(e.msg());
    }
}

}

