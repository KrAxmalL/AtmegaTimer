#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    atmegaTimer(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
{
    ui->setupUi(this);

    experiments();
    init();
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::initConnections()
{
    connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::startButtonPressed);
    connect(ui->stopButton, &QPushButton::pressed, this, &MainWindow::stopButtonPressed);
    connect(&timer, &QTimer::timeout, this, &MainWindow::executeProgram);

    connect(ui->cs10Button, &QPushButton::pressed, this, &MainWindow::changeCs10);
    connect(ui->clkButton, &QPushButton::pressed, this, &MainWindow::getClk);
}

void MainWindow::init()
{

    //init timer
    timer.setInterval(200);


    //init lines for graphicsView
    ox = QLine(x1, y1, x1, y1 + 300);
    oy = QLine(x1, y1 + 300, x1 + 300, y1 + 300);

    //init graphicsView

    QGraphicsView* view = ui->graphics;
    scene = new QGraphicsScene(view);
    view->setScene(scene);

    scene->addLine(ox);
    scene->addLine(oy);

    QGraphicsScene* scene = view->scene();

    initConnections();
}

void MainWindow::experiments()
{
    //setting text on QPlainText
    ui->cs11Label->setText({"CS11: " + QString::number(atmegaTimer.cs11())});
    //can show bools as integers
    //ui->cs11Text->setPlainText(QString::number(false));
    //button disabling
    //ui->cs10Button->setEnabled(false);

    isGraphicDrowing = false;
}

void MainWindow::executeProgram()
{
    bool finished = false;
    xPrev = xNew;
    yPrev = yNew;
    xNew += 1;
    yNew -= 1;

    //boolLock.lock();
    if(isGraphicDrowing /*&& !finished*/)
    {
        //boolLock.unlock();
        if(yPrev <= y1)
        {
            finished = true;
            yNew = -300;
            xNew = xPrev;
        }
        //QLine* ln = new QLine(x1, y1, x2, y2);
        scene->addLine(QLine(xPrev, yPrev, xNew, yNew), QPen(Qt::GlobalColor::black));
        //ui->cs10Text->setPlainText("Drawed line on: " + QString::number(x1));

        //ui->graphics->scene()->update();
        //ui->graphics->update();
        //delete ln;
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //QApplication::processEvents();
    }
    //boolLock.unlock();
}

void MainWindow::startButtonPressed()
{
    //boolLock.lock();
    isGraphicDrowing = true;
    ui->startButton->setEnabled(false);
    //executeProgram();
    timer.start();
    //boolLock.unlock();
    //std::thread thr(MainWindow::executeProgram, std::ref(*scene));
}

void MainWindow::stopButtonPressed()
{
    //boolLock.lock();
    timer.stop();
    ui->startButton->setEnabled(true);
    isGraphicDrowing = false;
    //boolLock.unlock();
    //ui->graphics->scene()->clear();
}

void MainWindow::changeCs10()
{
    bool newVal = !atmegaTimer.cs10();
    atmegaTimer.setCs10(newVal);
    ui->cs10Label->setText(QString("CS10: " + QString::number(newVal)));
}

void MainWindow::getClk()
{
    QString text = QInputDialog::getText(this, "Enter clk", "Enter clk: ");
    frequency = stol(text.toStdString());
    ui->clkLabel->setText(QString("CLK: " + QString::number(frequency)));

    //in milliseconds
    long periodToDraw =  1000 * (1.0 / ((double)frequency));
    timer.setInterval(std::chrono::milliseconds(periodToDraw));
}

