#ifndef GRAPHICDRAWER_H
#define GRAPHICDRAWER_H

#include <QWidget>
#include <QGraphicsView>
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

public slots:

    void drawNextLines();

private:

    QGraphicsScene& _scene;

    AtmegaTimer& _atmegaTimer;

    static constexpr int _drawingSteps = 300;
    int _performedSteps;

    //coordinates of ox and oy
    static constexpr qreal _ox0 = -400;
    static constexpr qreal _oy0 = 680;
    static constexpr qreal _ox1 = 400;
    static constexpr qreal _oy1 = 0;

    //coordinates for tcnt graphic
    static constexpr qreal _tcntStartX = _ox0;
    static constexpr qreal _tcntStartY = _oy0 - 100;

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
    static constexpr qreal _oc1aStartY = _oy0 - 50;

    qreal _oc1aPrevX;
    qreal _oc1aPrevY;

    qreal _oc1aNewX;
    qreal _oc1aNewY;

    qreal oc1aDistY;

    bool _reachedTop;

    QHash<int, qreal> _yCoordinateToBorder;

    void init();

    void drawNextNormalMode();
    void drawNextCtcMode();
    void drawNextFastPwmMode();
    void drawNextPhaseCorrectPwmMode();
    void drawNextPhaseFrequencyCorrectMode();

    void buildCoordinateYMap();
    void buildCoordinateXMap();

    int getNeededTopValue();
};

#endif // GRAPHICDRAWER_H
