#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>

namespace Ui {
class MainWindow;
}

namespace Student{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow* getInstance(QWidget* parent = 0);

    void drawHex();

public slots:
    void numberOfPlayers(int count);

private:
    Ui::MainWindow *ui;
    static MainWindow *mainInstance;
    QGraphicsScene *boardScene;
    void initScene();

};
}


#endif // MAINWINDOW_HH
