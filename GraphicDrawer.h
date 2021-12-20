#ifndef GRAPHICDRAWER_H
#define GRAPHICDRAWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QDebug>

#include "AtmegaTimer.h"

class GraphicDrawer : public QWidget
{
    Q_OBJECT
public:

    explicit GraphicDrawer(QGraphicsScene& scene, AtmegaTimer& atmegaTimer, QWidget *parent = nullptr);
    ~GraphicDrawer();

    void setStartingState();
    void buildCoordinates();
    void updateCoordinates();

public slots:

    void drawNextLines();

private:

    QGraphicsScene& _scene;

    AtmegaTimer& _atmegaTimer;

    static constexpr int _drawingSteps = 300;
    int _performedSteps;
    int _actualSteps = -1;

    //coordinates of ox and oy
    static constexpr qreal _ox0 = -400;
    static constexpr qreal _oy0 = 680;
    static constexpr qreal _ox1 = 400;
    static constexpr qreal _oy1 = 0;

    //coordinates for tcnt graphic
    static constexpr qreal _tcntStartX = _ox0;
    static constexpr qreal _tcntStartY = _oy0 - 110;

    qreal _tcntPrevX;
    qreal _tcntPrevY;

    qreal _tcntNewX;
    qreal _tcntNewY;

    //horizontal distance between time points (1 second)
    static constexpr qreal _xDistanceBetweenPoints = 200;
    qreal _xDelta;
    qreal _yDelta;

    //coordinates for oc1a graphic
    static constexpr qreal _oc1aStartX = _ox0;
    static constexpr qreal _oc1aStartY = _oy0 - 20;

    qreal _oc1aPrevX;
    qreal _oc1aPrevY;

    qreal _oc1aNewX;
    qreal _oc1aNewY;

    qreal _oc1aYDelta = -20;
    qreal _oc1aXDelta = 0;

    //coordinates for oc1b graphic
    static constexpr qreal _oc1bStartX = _ox0;
    static constexpr qreal _oc1bStartY = _oy0 - 70;

    qreal _oc1bPrevX;
    qreal _oc1bPrevY;

    qreal _oc1bNewX;
    qreal _oc1bNewY;

    qreal _oc1bYDelta = -20;
    qreal _oc1bXDelta = 0;

    int _stepsToReachMax = 0;

    bool _reachedTop;

    bool _isGoingUp = true;

    QHash<int, qreal> _yCoordinateToBorder;

    void init();

    void drawNextNormalMode();
    void drawNextCtcMode();
    void drawNextFastPwmMode();
    void drawNextPhaseCorrectPwmMode();
    void drawNextPhaseFrequencyCorrectMode();

    void updateTcntCoordinates();
    void updateOc1aCoordinates();
    void updateOc1bCoordinates();

    void updateOc1a();
    void updateOc1b();
    void updateTov1();

    void dropTcntToBottom();
    void setTcntDecreasing();

    void buildCoordinateYMap();
    void buildCoordinateXMap();

    void determineDeltasA();
    void determineDeltasB();
    void refreshDeltasA();
    void refreshDeltasB();

    int getNeededTopValue();

    bool equal(qreal left, qreal right);
    bool inRange(qreal val, qreal low, qreal high);
};

#endif // GRAPHICDRAWER_H
