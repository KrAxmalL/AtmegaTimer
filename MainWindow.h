#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QInputDialog>

#include <thread>
#include <mutex>
#include <chrono>

#include "AtmegaTimer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void startButtonPressed();
    void stopButtonPressed();
    void changeCs10();
    void getClk();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;

    QLine ox;
    QLine oy;

    int x1 = -360;
    int y1 = 0;
    int x2 = 100;
    int y2 = 100;

    std::mutex boolLock;

    QTimer timer;

    int xPrev = x1;
    int yPrev = 300;

    int xNew = x1;
    int yNew = y1;

    void init();
    void initConnections();
    void experiments();
    void executeProgram();

    bool isGraphicDrowing;

    long frequency;

    AtmegaTimer atmegaTimer;

};
#endif // MAINWINDOW_H
