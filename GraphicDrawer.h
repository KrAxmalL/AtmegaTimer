#ifndef GRAPHICDRAWER_H
#define GRAPHICDRAWER_H

#include <QWidget>
#include <QGraphicsView>

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

signals:

private:

    QGraphicsScene& _scene;

    AtmegaTimer& _atmegaTimer;

    int _drawingSteps = 300;

    QGraphicsLineItem* _ox;
    QGraphicsLineItem* _oy;

    //coordinates of ox and oy
    qreal _ox0 = -400;
    qreal _oy0 = 680;
    qreal _ox1 = 400;
    qreal _oy1 = 0;

    //coordinates for tcnt graphic
    qreal _tcntStartX = _ox0;
    qreal _tcntStartY = _oy0 - 100;

    qreal _tcntPrevX = _tcntStartX;
    qreal _tcntPrevY = _tcntStartY;

    qreal _tcntNewX = _tcntStartX;
    qreal _tcntNewY = _tcntStartY;

    //horizontal distance between time points (1 second)
    qreal _xDistanceBetweenPoints = 100;
    qreal _movementsForOneCycle = 20;
    qreal _xDelta = 0;
    qreal _yDelta = 0;
    qreal _ySteps = 0;
    int _currentXMovements = 0;

    //coordinates for oc1a graphic
    qreal _oc1aStartX = _ox0;
    qreal _oc1aStartY = _oy0 - 50;

    qreal _oc1aPrevX = _oc1aStartX;
    qreal _oc1aPrevY = _oc1aStartY;

    qreal _oc1aNewX = _oc1aStartX;
    qreal _oc1aNewY = _oc1aStartY;

    qreal oc1aDistY = -20;

    QHash<int, int> _yCoordinateToBorder;

    void init();
};

#endif // GRAPHICDRAWER_H
