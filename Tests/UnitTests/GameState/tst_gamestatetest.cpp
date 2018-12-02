#include <QString>
#include <QtTest>

#include "gamestate.hh"
#include "igameboard.hh"

class GameStateTest : public QObject
{
    Q_OBJECT

public:
    GameStateTest();

private Q_SLOTS:
    void testConstructor();
    void testCurrentGamePhase();
    void testCurrentPlayer();
    void testAddPointsToPlayer();
    void testInitPoints();
    void testGetPointsVectorForPlot();
    void testCurrentGamePhaseString();
    void testSetSpinResult();
    void testGetSpinAnimal();
    void testGetSpinMoveCount();
    void testGetWinner();
    void testGetSpinsLeft();
    void testSetSpinsLeft();
    void testGetPlayerPoints();


private:
    Student::GameState state_;
};

GameStateTest::GameStateTest()
{
}

void GameStateTest::testConstructor()
{
    QVERIFY(state_.currentPlayer() == 1);
}

void GameStateTest::testCurrentGamePhase()
{
    QCOMPARE(state_.currentGamePhase(), Common::GamePhase::MOVEMENT);
    state_.changeGamePhase(Common::GamePhase::SINKING);
    QCOMPARE(state_.currentGamePhase(), Common::GamePhase::SINKING);
}

void GameStateTest::testCurrentPlayer()
{
    QCOMPARE(state_.currentPlayer(), 1);
    state_.changePlayerTurn(2);
    QCOMPARE(state_.currentPlayer(), 2);
}

void GameStateTest::testAddPointsToPlayer()
{
    state_.initPoints(2);
    QCOMPARE(state_.getPlayerPoints(state_.currentPlayer()), 0);
    state_.addPointsToPlayer(state_.currentPlayer(), 5);
    QCOMPARE(state_.getPlayerPoints(state_.currentPlayer()), 5);
}

void GameStateTest::testInitPoints()
{
    state_.initPoints(2);
    QCOMPARE(state_.getPlayerPoints(state_.currentPlayer()),0);
}

void GameStateTest::testGetPointsVectorForPlot()
{
    state_.initPoints(3);
    QVERIFY(state_.getPointsVectorForPlot().size() == 3);
    state_.addPointsToPlayer(state_.currentPlayer(), 6);
    QCOMPARE(state_.getPointsVectorForPlot().at(state_.currentPlayer()-1), "Player: " +
             std::to_string(state_.currentPlayer()) + "   Points: " + "6");
}

void GameStateTest::testCurrentGamePhaseString()
{
    std::string text = state_.currentGamePhaseString();
    std::string expected = "Sinking";
    QCOMPARE(text, expected);
}

void GameStateTest::testSetSpinResult()
{
    std::pair<std::string,std::string> spinpair = std::make_pair("dolphin", "D");
    state_.setSpinResult(spinpair);
    QCOMPARE(state_.getSpinAnimal(), std::string("dolphin"));
}

void GameStateTest::testGetSpinAnimal()
{
    std::pair<std::string,std::string> spinpair = std::make_pair("kraken", "D");
    state_.setSpinResult(spinpair);
    QVERIFY(state_.getSpinAnimal() == std::string("kraken"));
}

void GameStateTest::testGetSpinMoveCount()
{
    std::pair<std::string,std::string> spinpair = std::make_pair("kraken", "3");
    state_.setSpinResult(spinpair);
    QVERIFY(state_.getSpinMovecount() == std::string("3"));
}

void GameStateTest::testGetWinner()
{
    state_.initPoints(2);
    state_.addPointsToPlayer(state_.currentPlayer(), 5);
    QCOMPARE(state_.getWinner(), "Player " + std::to_string(state_.currentPlayer()) + " wins!");
}

void GameStateTest::testGetSpinsLeft()
{
    state_.setSpinsLeft(2);
    QVERIFY(state_.getSpinsLeft() == 2);
}

void GameStateTest::testSetSpinsLeft()
{
    state_.setSpinsLeft(1);
    QVERIFY(state_.getSpinsLeft() == 1);
}

void GameStateTest::testGetPlayerPoints()
{
    state_.initPoints(2);
    state_.addPointsToPlayer(2, 16);
    QCOMPARE(state_.getPlayerPoints(2), 16);
}







QTEST_APPLESS_MAIN(GameStateTest)

#include "tst_gamestatetest.moc"
