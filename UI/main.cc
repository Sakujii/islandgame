#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"
#include "mainwindow.hh"

#include <memory>
#include <QApplication>
#include <QDebug>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Student::MainWindow w;
    w.show();
    return a.exec();
}
