#include "GraphicDrawer.h"

GraphicDrawer::GraphicDrawer(QGraphicsScene& scene, AtmegaTimer& atmegaTimer,
                             QWidget* parent): QWidget(parent), _scene(scene),
                                               _atmegaTimer(atmegaTimer)
{
    init();
}


GraphicDrawer::~GraphicDrawer() {}

void GraphicDrawer::init()
{
    _ox = _scene.addLine(_ox0, _oy0, _ox1, _oy0);
    _oy = _scene.addLine(_ox0, _oy0, _ox0, _oy1);

    for(int i = 1; i < 5; ++i)
    {
        _scene.addLine(_ox0 + (i*_xDistanceBetweenPoints), _oy0 + 10, _ox0 + (i*_xDistanceBetweenPoints), _oy0 - 10);
    }
}

void GraphicDrawer::drawNextLines()
{
    _tcntPrevX = _tcntNewX;
    _tcntPrevY = _tcntNewY;

    _oc1aPrevX = _oc1aNewX;
    _oc1aPrevY = _oc1aNewY;

    //_tcntNewX += 1;
    _tcntNewX += _xDelta;
    _tcntNewY -= _yDelta;
    //_tcntNewY -= 1;

    //_oc1aNewX += 1;
    _oc1aNewX += _xDelta;
    _oc1aNewY = _oc1aPrevY;

    bool prevOc1a = _atmegaTimer.oc1a();

    if(_atmegaTimer.top() > _yCoordinateToBorder[_currentXMovements - 1] && _atmegaTimer.top() <= _yCoordinateToBorder[_currentXMovements] /*_tcntNewY <= _oy1*/)
    {
            _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
            emit _atmegaTimer.oc1aChanged();
            _tcntNewX = _tcntPrevX;
            _tcntNewY = _tcntStartY;

            _currentXMovements = 0;
                //finished = true;
    }
        //QLine* ln = new QLine(x1, y1, x2, y2);
    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));

    if(_atmegaTimer.oc1a() != prevOc1a)
    {
            _oc1aNewY += oc1aDistY;
            oc1aDistY = -oc1aDistY;
    }

    ++_currentXMovements;

    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::buildCoordinateYMap()
{
    //подумати над розширенням, бо для дуже малих значень таймера лінії нема або дуже мала
    //1. або будувати для кожного top своє співвідношення.
    int yStep = _atmegaTimer.top() / _drawingSteps; //build from maximum value of timer
    int value = 0;
    for(int i = 0; i < _drawingSteps - 1; ++i)
    {
        _yCoordinateToBorder.insert(i, value);
        value += yStep;
    }
    _yCoordinateToBorder.insert(_drawingSteps - 1, _atmegaTimer.top());

    qreal height = _tcntStartY;
    _yDelta = height / _drawingSteps; //must be timer max value - 65535
    _currentXMovements = 0;
}

void GraphicDrawer::buildCoordinateXMap()
{
    qreal secondsNeeded = _atmegaTimer.top() / _atmegaTimer.actualClk();
    qreal pixelsNeeded = _xDistanceBetweenPoints * secondsNeeded;
    _xDelta = pixelsNeeded / _drawingSteps;
}
