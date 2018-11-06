#ifndef DIALOG_HH
#define DIALOG_HH

#include "mainwindow.hh"
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    virtual void playersSignal();

signals:
    void accept(int);


private:
    Ui::Dialog *ui;
    void setPlayers();
    MainWindow *main;
};


#endif // DIALOG_HH
