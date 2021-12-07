#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _scene(new QGraphicsScene),
    _atmegaTimer(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    _graphicDrawer(*_scene, _atmegaTimer, parent)
{
    ui->setupUi(this);

    //experiments();
    init();
}

MainWindow::~MainWindow()
{
    delete _scene;
    delete ui;
}

void MainWindow::initConnections()
{
    //start and stop
    connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::startButtonPressed);
    connect(ui->stopButton, &QPushButton::pressed, this, &MainWindow::stopButtonPressed);

    //cs registers
    connect(ui->cs10Button, &QPushButton::pressed, this, &MainWindow::changeCs10);
    connect(ui->cs11Button, &QPushButton::pressed, this, &MainWindow::changeCs11);
    connect(ui->cs12Button, &QPushButton::pressed, this, &MainWindow::changeCs12);

    //wgm registers
    connect(ui->wgm10Button, &QPushButton::pressed, this, &MainWindow::changeWgm10);
    connect(ui->wgm11Button, &QPushButton::pressed, this, &MainWindow::changeWgm11);
    connect(ui->wgm12Button, &QPushButton::pressed, this, &MainWindow::changeWgm12);
    connect(ui->wgm13Button, &QPushButton::pressed, this, &MainWindow::changeWgm13);

    //com registers
    connect(ui->com1a0Button, &QPushButton::pressed, this, &MainWindow::changeCom1a0);
    connect(ui->com1a1Button, &QPushButton::pressed, this, &MainWindow::changeCom1a1);

    //clks
    connect(ui->clkButton, &QPushButton::pressed, this, &MainWindow::setClk);
    connect(ui->t1Button, &QPushButton::pressed, this, &MainWindow::setT1);

    //ocr1a and icr1
    connect(ui->ocr1aButton, &QPushButton::pressed, this, &MainWindow::setOcr1a);
    connect(ui->icr1Button, &QPushButton::pressed, this, &MainWindow::setIcr1);
    connect(&_atmegaTimer, &AtmegaTimer::topChanged, this, &MainWindow::changeTop);

    //overflows
    connect(&_atmegaTimer, &AtmegaTimer::tov1Changed, this, &MainWindow::changeTov1);
    connect(&_atmegaTimer, &AtmegaTimer::oc1aChanged, this, &MainWindow::changeOc1a);
    connect(&_atmegaTimer, &AtmegaTimer::actualClkChanged, this, &MainWindow::changeActualClk);

    //timers for drawing graphic and counter values
    //connect(&timer, &QTimer::timeout, this, &MainWindow::executeProgram);
    connect(&timer, &QTimer::timeout, &_graphicDrawer, &GraphicDrawer::drawNextLines);
}

void MainWindow::initLabels()
{
    ui->cs10Label->setText(QString("CS10: " + QString::number(_atmegaTimer.cs10())));
    ui->cs11Label->setText(QString("CS11: " + QString::number(_atmegaTimer.cs11())));
    ui->cs12Label->setText(QString("CS12: " + QString::number(_atmegaTimer.cs12())));

    ui->wgm10Label->setText(QString("WGM10: " + QString::number(_atmegaTimer.wgm10())));
    ui->wgm11Label->setText(QString("WGM11: " + QString::number(_atmegaTimer.wgm11())));
    ui->wgm12Label->setText(QString("WGM12: " + QString::number(_atmegaTimer.wgm12())));
    ui->wgm13Label->setText(QString("WGM13: " + QString::number(_atmegaTimer.wgm13())));

    ui->com1a0Label->setText(QString("COM1A0: " + QString::number(_atmegaTimer.com1a0())));
    ui->com1a1Label->setText(QString("COM1A1: " + QString::number(_atmegaTimer.com1a1())));

    ui->clkLabel->setText(QString("CLK: " + QString::number(_atmegaTimer.clk())));
    ui->t1Label->setText(QString("T1: " + QString::number(_atmegaTimer.t1())));
    ui->actualClkLabel->setText(QString("Actual CLK: " + QString::number(_atmegaTimer.actualClk())));

    ui->ocr1aLabel->setText(QString("OCR1A: " + QString::number(_atmegaTimer.ocr1a())));
    ui->icr1Label->setText(QString("ICR1: " + QString::number(_atmegaTimer.icr1())));

    ui->topLabel->setText("TOP: " + QString::number(_atmegaTimer.top()));
    ui->tov1Label->setText("TOV1: " + QString::number(_atmegaTimer.tov1()));
    ui->oc1aLabel->setText("OC1A: " + QString::number(_atmegaTimer.oc1a()));
}

void MainWindow::init()
{

    timer.setInterval(std::chrono::milliseconds(10));

    //init graphicsView

    QGraphicsView* view = ui->graphics;
    view->setScene(_scene);

    initLabels();
    initConnections();

}

void MainWindow::experiments()
{

}

void MainWindow::startButtonPressed()
{
    ui->startButton->setEnabled(false);

    //if actual clock is zero - fix and do something
    _graphicDrawer.buildCoordinateXMap();
    _graphicDrawer.buildCoordinateYMap();

    timer.start();
}

void MainWindow::stopButtonPressed()
{
    //boolLock.lock();
    //preciseTimer.stop();
    timer.stop();
    //counterTimer.stop();
    ui->startButton->setEnabled(true);
    //boolLock.unlock();
    //ui->graphics->scene()->clear();
}

void MainWindow::changeCs10()
{
    bool newVal = !_atmegaTimer.cs10();
    _atmegaTimer.setCs10(newVal);
    ui->cs10Label->setText(QString("CS10: " + QString::number(_atmegaTimer.cs10())));
}

void MainWindow::changeCs11()
{
    bool newVal = !_atmegaTimer.cs11();
    _atmegaTimer.setCs11(newVal);
    ui->cs11Label->setText(QString("CS11: " + QString::number(_atmegaTimer.cs11())));
}

void MainWindow::changeCs12()
{
    bool newVal = !_atmegaTimer.cs12();
    _atmegaTimer.setCs12(newVal);
    ui->cs12Label->setText(QString("CS12: " + QString::number(_atmegaTimer.cs12())));
}

void MainWindow::changeWgm10()
{
     bool newVal = !_atmegaTimer.wgm10();
     _atmegaTimer.setWgm10(newVal);
     ui->wgm10Label->setText(QString("WGM10: " + QString::number(_atmegaTimer.wgm10())));
}

void MainWindow::changeWgm11()
{
    bool newVal = !_atmegaTimer.wgm11();
    _atmegaTimer.setWgm11(newVal);
    ui->wgm11Label->setText(QString("WGM11: " + QString::number(_atmegaTimer.wgm11())));
}

void MainWindow::changeWgm12()
{
    bool newVal = !_atmegaTimer.wgm12();
    _atmegaTimer.setWgm12(newVal);
    ui->wgm12Label->setText(QString("WGM12: " + QString::number(_atmegaTimer.wgm12())));
}

void MainWindow::changeWgm13()
{
    bool newVal = !_atmegaTimer.wgm13();
    _atmegaTimer.setWgm13(newVal);
    ui->wgm13Label->setText(QString("WGM13: " + QString::number(_atmegaTimer.wgm13())));
}

void MainWindow::changeCom1a0()
{
    bool newVal = !_atmegaTimer.com1a0();
    _atmegaTimer.setCom1a0(newVal);
    ui->com1a0Label->setText(QString("COM1A0: " + QString::number(_atmegaTimer.com1a0())));
}

void MainWindow::changeCom1a1()
{
    bool newVal = !_atmegaTimer.com1a1();
    _atmegaTimer.setCom1a1(newVal);
    ui->com1a1Label->setText(QString("COM1A1: " + QString::number(_atmegaTimer.com1a1())));
}

void MainWindow::setClk()
{
    bool isOkPressed = false;
    QString text = QInputDialog::getText(this, "Enter clk", "Enter clk: ", QLineEdit::Normal, "", &isOkPressed);
    if(isOkPressed)
    {
        try
        {
            long readedClk = stol(text.toStdString());
            if(readedClk < 0)
            {
                QMessageBox mb;
                mb.setText("Wrong input, value must be bigger than 0");
                mb.exec();
            }
            else
            {
                _atmegaTimer.setClk(readedClk);
                ui->clkLabel->setText(QString("CLK: " + QString::number(_atmegaTimer.clk())));
            }

        } catch (const std::invalid_argument& ex) {
            QMessageBox mb;
            mb.setText("Wrong input, only numbers allowed");
            mb.exec();

        } catch (const std::out_of_range& ex) {
            QMessageBox mb;
            mb.setText("Value is bigger, than max possible");
            mb.exec();
        }
    }
}

void MainWindow::setT1()
{
    bool isOkPressed = false;
    QString text = QInputDialog::getText(this, "Enter T1 value", "Enter T1 value: ", QLineEdit::Normal, "", &isOkPressed);
    if(isOkPressed)
    {
        try
        {
            long readedT1 = stol(text.toStdString());
            if(readedT1 < 0)
            {
                QMessageBox mb;
                mb.setText("Wrong input, value must be bigger than 0");
                mb.exec();
            }
            else
            {
                _atmegaTimer.setT1(readedT1);
                ui->t1Label->setText(QString("T1: " + QString::number(_atmegaTimer.t1())));
            }

        } catch (const std::invalid_argument& ex) {
            QMessageBox mb;
            mb.setText("Wrong input, only numbers allowed");
            mb.exec();

        } catch (const std::out_of_range& ex) {
            QMessageBox mb;
            mb.setText("Value is bigger, than max possible");
            mb.exec();
        }
    }
}

void MainWindow::changeActualClk()
{
    ui->actualClkLabel->setText("Actual CLK: " + QString::number(_atmegaTimer.actualClk()));
}

void MainWindow::setOcr1a()
{   
    bool isOkPressed = false;
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::IntInput);
    int newVal = dialog.getInt(this, "Enter OCR1A value", "Enter OCR1A value: ", 0, 0, 65535, 1, &isOkPressed);
    if(isOkPressed)
    {
        if(newVal < WaveFormGenerator::Bottom || newVal > WaveFormGenerator::Max)
        {
            QMessageBox mb;
            mb.setText("Wrong input, enter value between " + QString::number(WaveFormGenerator::Bottom)
                       + " and " + QString::number(WaveFormGenerator::Max));
            mb.exec();
        }
        else
        {
            _atmegaTimer.setOcr1a(newVal);
            ui->ocr1aLabel->setText(QString("OCR1A: " + QString::number(_atmegaTimer.ocr1a())));
        }
    }
}

void MainWindow::setIcr1()
{
    timer.stop();

    bool isOkPressed = false;
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::IntInput);
    int newVal = dialog.getInt(this, "Enter ICR1 value", "Enter ICR1 value: ", 0, 0, 65535, 1, &isOkPressed);
    if(isOkPressed)
    {
        if(newVal < WaveFormGenerator::Bottom || newVal > WaveFormGenerator::Max)
        {
            QMessageBox mb;
            mb.setText("Wrong input, enter value between " + QString::number(WaveFormGenerator::Bottom)
                       + " and " + QString::number(WaveFormGenerator::Max));
            mb.exec();
        }
        else
        {
            _atmegaTimer.setIcr1(newVal);
            ui->icr1Label->setText(QString("ICR1: " + QString::number(_atmegaTimer.icr1())));
        }
    }

    timer.start();
}

void MainWindow::changeTop()
{
    ui->topLabel->setText("TOP: " + QString::number(_atmegaTimer.top()));
}

void MainWindow::changeTov1()
{
    ui->tov1Label->setText("TOV1: " + QString::number(_atmegaTimer.tov1()));
}

void MainWindow::changeOc1a()
{
    ui->oc1aLabel->setText("OC1A: " + QString::number(_atmegaTimer.oc1a()));
}
