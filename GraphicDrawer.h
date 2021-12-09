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

    void buildCoordinateYMap();
    void buildCoordinateXMap();

public slots:

    void drawNextLines();

    void drawNextNormalMode();
    void drawNextCtcMode();
    void drawNextFastPwmMode();
    void drawNextPhaseCorrectPwmMode();
    void drawNextPhaseFrequencyCorrectMode();

signals:

private:

    QGraphicsScene& _scene;

    AtmegaTimer& _atmegaTimer;

    static constexpr int _drawingSteps = 300;
    int _performedSteps = 0;

    QGraphicsLineItem* _ox;
    QGraphicsLineItem* _oy;

    //coordinates of ox and oy
    static constexpr qreal _ox0 = -400;
    static constexpr qreal _oy0 = 680;
    static constexpr qreal _ox1 = 400;
    static constexpr qreal _oy1 = 0;

    //coordinates for tcnt graphic
    static constexpr qreal _tcntStartX = _ox0;
    static constexpr qreal _tcntStartY = _oy0 - 100;

    qreal _tcntPrevX = _tcntStartX;
    qreal _tcntPrevY = _tcntStartY;

    qreal _tcntNewX = _tcntStartX;
    qreal _tcntNewY = _tcntStartY;

    //horizontal distance between time points (1 second)
    static constexpr qreal _xDistanceBetweenPoints = 200;
    qreal _xDelta = 0;
    qreal _yDelta = 0;
    //qreal _ySteps = 0;

    //coordinates for oc1a graphic
    static constexpr qreal _oc1aStartX = _ox0;
    static constexpr qreal _oc1aStartY = _oy0 - 50;

    qreal _oc1aPrevX = _oc1aStartX;
    qreal _oc1aPrevY = _oc1aStartY;

    qreal _oc1aNewX = _oc1aStartX;
    qreal _oc1aNewY = _oc1aStartY;

    qreal oc1aDistY = -20;

    bool _reachedTop = false;

    QHash<int, qreal> _yCoordinateToBorder;

    void init();
};

#endif // GRAPHICDRAWER_H
