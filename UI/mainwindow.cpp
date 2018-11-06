#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "dialog.hh"


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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numberOfPlayers(int count)
{
    ui->label->setText(QString::number(count));
}

