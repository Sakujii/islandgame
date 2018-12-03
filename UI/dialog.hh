#ifndef DIALOG_HH
#define DIALOG_HH

#include "mainwindow.hh"
#include <QDialog>

/**
 * @file
 * @brief Implements dialog class
 */

namespace Ui {
class Dialog;
}

namespace Student {

/**
 * @brief Offers a class to draw a dialog
 * to ask for number of players and start the game.
 */
class Dialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QWidget for dialog
     */
    explicit Dialog(QWidget *parent = 0);

    /**
     * @brief Destructor
     * @post ui of Dialog is deleted
     */
    ~Dialog();

public slots:
    /**
     * @brief Virtual slot for changing player amount
     * @post ui of Dialog is deleted
     */
    virtual void playersSignal();

signals:
    /**
     * @brief Signal for accepting the dialog
     * @param players count to emit
     */
    void acceptSignal(int players);

private:

    //! Pointer to dialog ui
    Ui::Dialog *ui;

};
}


#endif // DIALOG_HH
