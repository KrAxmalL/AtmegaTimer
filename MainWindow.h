#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>

#include "AtmegaTimer.h"
#include "GraphicDrawer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

public slots:

    void startButtonPressed();
    void stopButtonPressed();
    void clearButtonPressed();

    //clock select registers change
    void changeCs10();
    void changeCs11();
    void changeCs12();

    //wage generation registers change
    void changeWgm10();
    void changeWgm11();
    void changeWgm12();
    void changeWgm13();

    //compare output registers change
    void changeCom1a0();
    void changeCom1a1();
    void changeCom1b0();
    void changeCom1b1();

    //clock frequency setting
    void setClk();
    void setT1();
    void changeActualClk();

    //compare registers setting
    void setOcr1a();
    void setOcr1b();
    void setIcr1();
    void changeTop();

    //overflow and compare output flags change
    void changeTov1();
    void changeOc1a();
    void changeOc1b();

private:

    Ui::MainWindow *ui;

    QGraphicsScene* _scene;

    AtmegaTimer _atmegaTimer;

    GraphicDrawer _graphicDrawer;

    QTimer timer;

    bool _stopped = false;
    bool _isDrawing = false;

    void init();
    void initLabels();
    void initConnections();

    void disableRuntimeChangingButtons();
    void enableRuntimeChangingButtons();

    int getMinOcr1aValue();
    int getMinIcr1Value();

    void setStartButtonState();

    void recalculateCurrentRegisterValue();
};
#endif // MAINWINDOW_H
