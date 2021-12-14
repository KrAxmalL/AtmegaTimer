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

    init();
}

MainWindow::~MainWindow()
{
    delete _scene;
    delete ui;
}

void MainWindow::initConnections()
{
    //start, stop and clear buttons
    connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::startButtonPressed);
    connect(ui->stopButton, &QPushButton::pressed, this, &MainWindow::stopButtonPressed);
    connect(ui->clearButton, &QPushButton::pressed, this, &MainWindow::clearButtonPressed);

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

    setStartButtonState();

}

void MainWindow::startButtonPressed()
{
    ui->startButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    disableRuntimeChangingButtons();
    if(!_stopped)
    {
        _graphicDrawer.buildCoordinates();
    }
    _stopped = false;
    _isDrawing = true;
    timer.start();
}

void MainWindow::stopButtonPressed()
{
    if(!_stopped)
    {
        timer.stop();
        _stopped = true;
        ui->clearButton->setEnabled(true);
        setStartButtonState();
    }
}

void MainWindow::clearButtonPressed()
{
    _atmegaTimer.setOc1a(0);
    changeOc1a();
    _atmegaTimer.setTov1(0);
    changeTov1();
    _graphicDrawer.setStartingState();
    enableRuntimeChangingButtons();
    setStartButtonState();
    _stopped = false;
    _isDrawing = false;
}

void MainWindow::changeCs10()
{
    bool newVal = !_atmegaTimer.cs10();
    _atmegaTimer.setCs10(newVal);
    ui->cs10Label->setText(QString("CS10: " + QString::number(_atmegaTimer.cs10())));

    setStartButtonState();
}

void MainWindow::changeCs11()
{
    bool newVal = !_atmegaTimer.cs11();
    _atmegaTimer.setCs11(newVal);
    ui->cs11Label->setText(QString("CS11: " + QString::number(_atmegaTimer.cs11())));

    setStartButtonState();
}

void MainWindow::changeCs12()
{
    bool newVal = !_atmegaTimer.cs12();
    _atmegaTimer.setCs12(newVal);
    ui->cs12Label->setText(QString("CS12: " + QString::number(_atmegaTimer.cs12())));

    setStartButtonState();
}

void MainWindow::changeWgm10()
{
     bool newVal = !_atmegaTimer.wgm10();
     _atmegaTimer.setWgm10(newVal);
     ui->wgm10Label->setText(QString("WGM10: " + QString::number(_atmegaTimer.wgm10())));

     setStartButtonState();
     recalculateCurrentRegisterValue();
}

void MainWindow::changeWgm11()
{
    bool newVal = !_atmegaTimer.wgm11();
    _atmegaTimer.setWgm11(newVal);
    ui->wgm11Label->setText(QString("WGM11: " + QString::number(_atmegaTimer.wgm11())));

    setStartButtonState();
    recalculateCurrentRegisterValue();
}

void MainWindow::changeWgm12()
{
    bool newVal = !_atmegaTimer.wgm12();
    _atmegaTimer.setWgm12(newVal);
    ui->wgm12Label->setText(QString("WGM12: " + QString::number(_atmegaTimer.wgm12())));

    setStartButtonState();
    recalculateCurrentRegisterValue();
}

void MainWindow::changeWgm13()
{
    bool newVal = !_atmegaTimer.wgm13();
    _atmegaTimer.setWgm13(newVal);
    ui->wgm13Label->setText(QString("WGM13: " + QString::number(_atmegaTimer.wgm13())));

    setStartButtonState();
    recalculateCurrentRegisterValue();
}

void MainWindow::changeCom1a0()
{
    bool newVal = !_atmegaTimer.com1a0();
    _atmegaTimer.setCom1a0(newVal);
    ui->com1a0Label->setText(QString("COM1A0: " + QString::number(_atmegaTimer.com1a0())));

    setStartButtonState();
}

void MainWindow::changeCom1a1()
{
    bool newVal = !_atmegaTimer.com1a1();
    _atmegaTimer.setCom1a1(newVal);
    ui->com1a1Label->setText(QString("COM1A1: " + QString::number(_atmegaTimer.com1a1())));

    setStartButtonState();
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
                setStartButtonState();
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
                setStartButtonState();
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
    timer.stop();

    bool isOkPressed = false;
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::IntInput);
    int newVal = dialog.getInt(this, "Enter OCR1A value", "Enter OCR1A value: ", 10, getMinOcr1aValue(), WaveFormGenerator::Max, 1, &isOkPressed);
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
            if(!_isDrawing)
            {
                _atmegaTimer.loadOcr1aFromBuffer();
            }
            _graphicDrawer.buildCoordinates();
            ui->ocr1aLabel->setText(QString("OCR1A: " + QString::number(_atmegaTimer.ocr1aBuffer())));
            setStartButtonState();
        }
    }
}

void MainWindow::setIcr1()
{
    timer.stop();

    bool isOkPressed = false;
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::IntInput);
    int newVal = dialog.getInt(this, "Enter ICR1 value", "Enter ICR1 value: ", 10, getMinIcr1Value(), WaveFormGenerator::Max, 1, &isOkPressed);
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
            _graphicDrawer.buildCoordinates();
            setStartButtonState();
        }
    }
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

void MainWindow::disableRuntimeChangingButtons()
{
    ui->clkButton->setEnabled(false);
    ui->com1a0Button->setEnabled(false);
    ui->com1a1Button->setEnabled(false);
    ui->cs10Button->setEnabled(false);
    ui->cs11Button->setEnabled(false);
    ui->cs12Button->setEnabled(false);
    ui->t1Button->setEnabled(false);
    ui->wgm10Button->setEnabled(false);
    ui->wgm11Button->setEnabled(false);
    ui->wgm12Button->setEnabled(false);
    ui->wgm13Button->setEnabled(false);
}

void MainWindow::enableRuntimeChangingButtons()
{
    ui->clkButton->setEnabled(true);
    ui->com1a0Button->setEnabled(true);
    ui->com1a1Button->setEnabled(true);
    ui->cs10Button->setEnabled(true);
    ui->cs11Button->setEnabled(true);
    ui->cs12Button->setEnabled(true);
    ui->t1Button->setEnabled(true);
    ui->wgm10Button->setEnabled(true);
    ui->wgm11Button->setEnabled(true);
    ui->wgm12Button->setEnabled(true);
    ui->wgm13Button->setEnabled(true);
}

int MainWindow::getMinOcr1aValue()
{
    switch(_atmegaTimer.timerMode())
    {
        case WaveFormGenerator::Mode::PWMPhOcr:
        case WaveFormGenerator::Mode::PWMPhFrOcr:
        case WaveFormGenerator::Mode::FastPWMOcr: return 3;

        case WaveFormGenerator::Mode::FastPWM8:
        case WaveFormGenerator::Mode::FastPWM9:
        case WaveFormGenerator::Mode::FastPWM10:
        case WaveFormGenerator::Mode::FastPWMIcr:
        case WaveFormGenerator::Mode::PWM8Ph:
        case WaveFormGenerator::Mode::PWM9Ph:
        case WaveFormGenerator::Mode::PWM10Ph:
        case WaveFormGenerator::Mode::PWMPhIcr:
        case WaveFormGenerator::Mode::PWMPhFrIcr:
        case WaveFormGenerator::Mode::Normal:
        case WaveFormGenerator::Mode::CTCOcr:
        case WaveFormGenerator::Mode::CTCIcr:
        case WaveFormGenerator::Mode::Reserved: return 0;
    }
}

int MainWindow::getMinIcr1Value()
{
    switch(_atmegaTimer.timerMode())
    {
        case WaveFormGenerator::Mode::PWMPhIcr:
        case WaveFormGenerator::Mode::PWMPhFrIcr:
        case WaveFormGenerator::Mode::FastPWMIcr: return 3;

        case WaveFormGenerator::Mode::FastPWM8:
        case WaveFormGenerator::Mode::FastPWM9:
        case WaveFormGenerator::Mode::FastPWM10:
        case WaveFormGenerator::Mode::FastPWMOcr:
        case WaveFormGenerator::Mode::PWM8Ph:
        case WaveFormGenerator::Mode::PWM9Ph:
        case WaveFormGenerator::Mode::PWM10Ph:
        case WaveFormGenerator::Mode::PWMPhOcr:
        case WaveFormGenerator::Mode::PWMPhFrOcr:
        case WaveFormGenerator::Mode::Normal:
        case WaveFormGenerator::Mode::CTCOcr:
        case WaveFormGenerator::Mode::CTCIcr:
        case WaveFormGenerator::Mode::Reserved: return 0;
    }
}

void MainWindow::setStartButtonState()
{
    if(_atmegaTimer.actualClk() <= 0
       || _atmegaTimer.timerMode() == WaveFormGenerator::Mode::Reserved
       || _atmegaTimer.timerState() == ClockSelect::State::Off)
    {
        ui->startButton->setEnabled(false);
    }
    else
    {
        ui->startButton->setEnabled(true);
    }
}

void MainWindow::recalculateCurrentRegisterValue()
{
    int ocr1aMin = getMinOcr1aValue();
    if(_atmegaTimer.ocr1a() < ocr1aMin)
    {
        _atmegaTimer.setOcr1a(ocr1aMin);
         ui->ocr1aLabel->setText(QString("OCR1A: " + QString::number(_atmegaTimer.ocr1a())));
    }

    int icr1Min = getMinIcr1Value();
    if(_atmegaTimer.icr1() < icr1Min)
    {
        _atmegaTimer.setIcr1(icr1Min);
         ui->icr1Label->setText(QString("ICR1: " + QString::number(_atmegaTimer.icr1())));
    }
}
