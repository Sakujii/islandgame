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
    void testCase1();
    void testCurrentGamePhase();
    void testCurrentPlayer();

private:
    Student::GameState state_;
};

GameStateTest::GameStateTest()
{
}

void GameStateTest::testCase1()
{
    QVERIFY2(true, "Failure");
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


QTEST_APPLESS_MAIN(GameStateTest)

#include "tst_gamestatetest.moc"
