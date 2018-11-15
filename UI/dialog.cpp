#include "dialog.hh"
#include "ui_dialog.h"
#include "mainwindow.hh"

#include <iostream>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &Dialog::playersSignal);
    connect(ui->exitButton, &QPushButton::clicked, this, &Dialog::reject);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::playersSignal()
{
    int count = ui->spinBox->value();
    emit accept(count);
    this->reject();
}


