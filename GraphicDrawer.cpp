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

void GraphicDrawer::drawNextNormalMode()
{
    if(_reachedTop)
    {
        _tcntNewX = _tcntPrevX;
        _tcntNewY = _tcntStartY;

        _performedSteps = 0;
        _reachedTop = false;
    }
    else
    {
        _tcntPrevX = _tcntNewX;
        _tcntPrevY = _tcntNewY;

        _tcntNewX += _xDelta;
        _tcntNewY -= _yDelta;

        if(_atmegaTimer.comparableValue() > _yCoordinateToBorder[_performedSteps - 1] && _atmegaTimer.comparableValue() <= _yCoordinateToBorder[_performedSteps])
        {
            _atmegaTimer.setTov1(true);
            emit _atmegaTimer.tov1Changed();
            _reachedTop = true;
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextCtcMode()
{
    if(_reachedTop)
    {
        _tcntNewX = _tcntPrevX;
        _tcntNewY = _tcntStartY;

        _performedSteps = 0;
        _reachedTop = false;
    }
    else
    {
        _tcntPrevX = _tcntNewX;
        _tcntPrevY = _tcntNewY;

        _tcntNewX += _xDelta;
        _tcntNewY -= _yDelta;

        _oc1aPrevX = _oc1aNewX;
        _oc1aPrevY = _oc1aNewY;

        _oc1aNewX += _xDelta;
        _oc1aNewY = _oc1aPrevY;

        if(false) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
        }

        bool prevOc1a = _atmegaTimer.oc1a();

        if((_atmegaTimer.comparableValue() > _yCoordinateToBorder[_performedSteps - 1]
            && _atmegaTimer.comparableValue() <= _yCoordinateToBorder[_performedSteps]))
        {
            _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
            emit _atmegaTimer.oc1aChanged();

            if(_atmegaTimer.oc1a() != prevOc1a)
            {
                _oc1aNewY += oc1aDistY;
                oc1aDistY = -oc1aDistY;
            }

            _reachedTop = true;
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextFastPwmMode()
{
    _tcntPrevX = _tcntNewX;
    _tcntPrevY = _tcntNewY;

    _tcntNewX += _xDelta;
    _tcntNewY -= _yDelta;

    _oc1aPrevX = _oc1aNewX;
    _oc1aPrevY = _oc1aNewY;

    _oc1aNewX += _xDelta;
    _oc1aNewY = _oc1aPrevY;

    bool prevOc1a = _atmegaTimer.oc1a();

    if(_atmegaTimer.comparableValue() > _yCoordinateToBorder[_performedSteps - 1] && _atmegaTimer.comparableValue() <= _yCoordinateToBorder[_performedSteps])
    {
            _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
            emit _atmegaTimer.oc1aChanged();
    }

    if(_atmegaTimer.top() > _yCoordinateToBorder[_performedSteps - 1] && _atmegaTimer.top() <= _yCoordinateToBorder[_performedSteps])
    {
            _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
            emit _atmegaTimer.oc1aChanged();
            _tcntNewX = _tcntPrevX;
            _tcntNewY = _tcntStartY;

            _performedSteps = 0;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));

    if(_atmegaTimer.oc1a() != prevOc1a)
    {
            _oc1aNewY += oc1aDistY;
            oc1aDistY = -oc1aDistY;
    }

    ++_performedSteps;

    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextPhaseCorrectPwmMode()
{

}

void GraphicDrawer::drawNextPhaseFrequencyCorrectMode()
{

}

void GraphicDrawer::drawNextLines()
{
    switch(_atmegaTimer.timerMode())
    {
        case WaveFormGenerator::Mode::Normal: drawNextNormalMode(); return;

        case WaveFormGenerator::Mode::CTCOcr:
        case WaveFormGenerator::Mode::CTCIcr: drawNextCtcMode(); return;

        case WaveFormGenerator::Mode::FastPWM8:
        case WaveFormGenerator::Mode::FastPWM9:
        case WaveFormGenerator::Mode::FastPWM10:
        case WaveFormGenerator::Mode::FastPWMOcr:
        case WaveFormGenerator::Mode::FastPWMIcr: drawNextFastPwmMode(); return;

        case WaveFormGenerator::Mode::PWM8Ph:
        case WaveFormGenerator::Mode::PWM9Ph:
        case WaveFormGenerator::Mode::PWM10Ph:
        case WaveFormGenerator::Mode::PWMPhOcr:
        case WaveFormGenerator::Mode::PWMPhIcr: drawNextPhaseCorrectPwmMode(); return;

        case WaveFormGenerator::Mode::PWMPhFrOcr:
        case WaveFormGenerator::Mode::PWMPhFrIcr: drawNextPhaseFrequencyCorrectMode(); return;

        case WaveFormGenerator::Mode::Reserved: return;
    }
}

void GraphicDrawer::buildCoordinateYMap()
{
    //подумати над розширенням, бо для дуже малих значень таймера лінії нема або дуже мала
    //1. або будувати для кожного top своє співвідношення.
    //int yStep = _atmegaTimer.top() / _drawingSteps; //build from maximum value of timer
    qreal yStep = (qreal)_atmegaTimer.top() / (qreal)_drawingSteps;

    //qInfo() << "YStep is: " << yStep << "\n";//int value = 0;

    qreal value = 0.0;
    for(int i = 0; i < _drawingSteps - 1; ++i)
    {
        _yCoordinateToBorder.insert(i, value);
        value += yStep;
    }
    _yCoordinateToBorder.insert(_drawingSteps - 1, _atmegaTimer.top());

    //qreal height = _tcntStartY;
    qreal height = (_tcntStartY / 65535.0) * (qreal)_atmegaTimer.top();
    _yDelta = height / (qreal)_drawingSteps; //must be timer max value - 65535
    _performedSteps = 0;
}

void GraphicDrawer::buildCoordinateXMap()
{
    //add the calculation for other modes
    qreal secondsNeeded = (qreal)_atmegaTimer.top() / (qreal)_atmegaTimer.actualClk();
    /*if(secondsNeeded == 0)
    {
        secondsNeeded = 1;
    }*/
    qreal pixelsNeeded = _xDistanceBetweenPoints * secondsNeeded;
    _xDelta = pixelsNeeded / (qreal)_drawingSteps;
}
