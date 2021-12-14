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
    setStartingState();
}

//працює првильно
void GraphicDrawer::drawNextNormalMode()
{
    _atmegaTimer.loadOcr1aFromBuffer();

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

        if(equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps]))
        {
            _atmegaTimer.setTov1(true);
            emit _atmegaTimer.tov1Changed();

            _reachedTop = true;
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
}

//працює првильно
//але додатково перевірити на малих та крайнії значеннях
void GraphicDrawer::drawNextCtcMode()
{
    _atmegaTimer.loadOcr1aFromBuffer();

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

        _oc1aNewX += _oc1aXDelta;
        _oc1aNewY = _oc1aPrevY;

        if(_stepsToReachMax == _performedSteps) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
        }

        if(_performedSteps <= _drawingSteps && equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
        {
            /*if(_atmegaTimer.com1a0() && !_atmegaTimer.com1a1())
            {
                _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());

                _oc1aNewY += oc1aDistY;
                oc1aDistY = -oc1aDistY;
                _oc1aNewX = _oc1aPrevX;
            }
            else if(!_atmegaTimer.com1a0() && _atmegaTimer.com1a1())
            {
                _atmegaTimer.setOc1a(false);

                //_oc1aNewY += oc1aDistY;
                oc1aDistY = -oc1aDistY;
                _oc1aNewX = _oc1aPrevX;
            }
            else if(_atmegaTimer.com1a0() && _atmegaTimer.com1a1())
            {
                _atmegaTimer.setOc1a(true);

                //_oc1aNewY += oc1aDistY;
                oc1aDistY = -oc1aDistY;
                _oc1aNewX = _oc1aPrevX;
            }*/

            _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
            emit _atmegaTimer.oc1aChanged();
            _reachedTop = true;

            _oc1aNewY += _oc1aYDelta;
            _oc1aYDelta = -_oc1aYDelta;
            _oc1aNewX = _oc1aPrevX;
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    qInfo() << "newY is: " << (_tcntNewY - _yDelta) << "\n";
    qInfo() << "drawed line is: " << _tcntPrevX << " " << _tcntPrevY << " " << _tcntNewX << " " << _tcntNewY << "\n";
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));

}

//додатково перевірити
void GraphicDrawer::drawNextFastPwmMode()
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

        _oc1aNewX += _oc1aXDelta;
        _oc1aNewY = _oc1aPrevY;

        if(_stepsToReachMax == _performedSteps) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool reached = false;

            if(_performedSteps == 0)
            {
                _atmegaTimer.loadOcr1aFromBuffer();

                reached = _atmegaTimer.comparableValue() <=  _yCoordinateToBorder[_performedSteps];
            }
            else
            {
                reached = inRange(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(reached/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                emit _atmegaTimer.oc1aChanged();

                _oc1aNewY += _oc1aYDelta;
                _oc1aYDelta = -_oc1aYDelta;

                _oc1aNewX = _oc1aPrevX;

                qInfo() << "comparable value is: " << (qreal)_atmegaTimer.comparableValue() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {
                _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                emit _atmegaTimer.oc1aChanged();

                _atmegaTimer.setTov1(1);
                emit _atmegaTimer.tov1Changed();

                _reachedTop = true;

                _oc1aNewY += _oc1aYDelta;
                _oc1aYDelta = -_oc1aYDelta;

                _oc1aNewX = _oc1aPrevX;

                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";
            }
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextPhaseCorrectPwmMode()
{
    //зробити константні виходи якщо ocr1a == 0 або топ
    if(_reachedTop)
    {
        _tcntNewX = _tcntPrevX;
        _tcntNewY = _tcntPrevY;

        _isGoingUp = false;
        _reachedTop = false;

        _yDelta = -_yDelta;

        _atmegaTimer.loadOcr1aFromBuffer();
    }
    else
    {
        _tcntPrevX = _tcntNewX;
        _tcntPrevY = _tcntNewY;

        _tcntNewX += _xDelta;
        _tcntNewY -= _yDelta;

        _oc1aPrevX = _oc1aNewX;
        _oc1aPrevY = _oc1aNewY;

        _oc1aNewX += _oc1aXDelta;
        _oc1aNewY = _oc1aPrevY;

        if(_stepsToReachMax == _performedSteps && _isGoingUp) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
            qInfo() << "reached top through steps: " << _performedSteps << "\n";
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool reached = false;

            if(_performedSteps == 0)
            {
                reached = false/*_atmegaTimer.comparableValue() <=  _yCoordinateToBorder[_performedSteps]*/;

                if(!_isGoingUp)
                {
                    _yDelta = -_yDelta;
                    _isGoingUp = true;
                }
            }
            else
            {
                reached = inRange(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(reached/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                emit _atmegaTimer.oc1aChanged();

                _oc1aNewY += _oc1aYDelta;
                _oc1aYDelta = -_oc1aYDelta;

                _oc1aNewX = _oc1aPrevX;

                qInfo() << "comparable value is: " << (qreal)_atmegaTimer.comparableValue() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {

                qInfo() << "reached top in phase correct with steps: " << _performedSteps << "\n";
                //_atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                //emit _atmegaTimer.oc1aChanged();

                //_atmegaTimer.setTov1(1);
                //emit _atmegaTimer.tov1Changed();

                if(_isGoingUp)
                {
                    _reachedTop = true;
                }

                if(reached)
                {
                    _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                    emit _atmegaTimer.oc1aChanged();

                    _oc1aNewY += _oc1aYDelta;
                    _oc1aYDelta = -_oc1aYDelta;

                    _oc1aNewX = _oc1aPrevX;
                }

                //_oc1aNewY += oc1aDistY;
                //oc1aDistY = -oc1aDistY;

                //_oc1aNewX = _oc1aPrevX;

                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";
            }
        }

        if(_isGoingUp && !_reachedTop)
        {
            ++_performedSteps;
        }
        else
        {
            --_performedSteps;
        }
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextPhaseFrequencyCorrectMode()
{
    //зробити константні виходи якщо ocr1a == 0 або топ
    if(_reachedTop)
    {
        _tcntNewX = _tcntPrevX;
        _tcntNewY = _tcntPrevY;

        _isGoingUp = false;
        _reachedTop = false;

        _yDelta = -_yDelta;
    }
    else
    {
        _tcntPrevX = _tcntNewX;
        _tcntPrevY = _tcntNewY;

        _tcntNewX += _xDelta;
        _tcntNewY -= _yDelta;

        _oc1aPrevX = _oc1aNewX;
        _oc1aPrevY = _oc1aNewY;

        _oc1aNewX += _oc1aXDelta;
        _oc1aNewY = _oc1aPrevY;

        if(_stepsToReachMax == _performedSteps && _isGoingUp) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
            qInfo() << "reached top through steps: " << _performedSteps << "\n";
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool reached = false;

            if(_performedSteps == 0)
            {
                reached = false/*_atmegaTimer.comparableValue() <=  _yCoordinateToBorder[_performedSteps]*/;

                if(!_isGoingUp)
                {
                    _yDelta = -_yDelta;
                    _isGoingUp = true;
                    _atmegaTimer.loadOcr1aFromBuffer();
                }
            }
            else
            {
                reached = inRange(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(reached/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                emit _atmegaTimer.oc1aChanged();

                _oc1aNewY += _oc1aYDelta;
                _oc1aYDelta = -_oc1aYDelta;

                _oc1aNewX = _oc1aPrevX;

                qInfo() << "comparable value is: " << (qreal)_atmegaTimer.comparableValue() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {

                qInfo() << "reached top in phase correct with steps: " << _performedSteps << "\n";
                //_atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                //emit _atmegaTimer.oc1aChanged();

                //_atmegaTimer.setTov1(1);
                //emit _atmegaTimer.tov1Changed();

                if(_isGoingUp)
                {
                    _reachedTop = true;
                }

                if(reached)
                {
                    _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
                    emit _atmegaTimer.oc1aChanged();

                    _oc1aNewY += _oc1aYDelta;
                    _oc1aYDelta = -_oc1aYDelta;

                    _oc1aNewX = _oc1aPrevX;
                }

                //_oc1aNewY += oc1aDistY;
                //oc1aDistY = -oc1aDistY;

                //_oc1aNewX = _oc1aPrevX;

                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";
            }
        }

        if(_isGoingUp && !_reachedTop)
        {
            ++_performedSteps;
        }
        else
        {
            --_performedSteps;
        }
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
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

void GraphicDrawer::setStartingState()
{
    _scene.clear();

    _scene.addLine(_ox0, _oy0, _ox1, _oy0);
    _scene.addLine(_ox0, _oy0, _ox0, _oy1);

    for(int i = 1; i < 5; ++i)
    {
        _scene.addLine(_ox0 + (i*_xDistanceBetweenPoints), _oy0 + 10, _ox0 + (i*_xDistanceBetweenPoints), _oy0 - 10);
    }

    _xDelta = 0;
    _yDelta = 0;

    _performedSteps = 0;

    _reachedTop = false;
    _isGoingUp = true;

    _tcntPrevX = _tcntStartX;
    _tcntPrevY = _tcntStartY;
    _tcntNewX = _tcntStartX;
    _tcntNewY = _tcntStartY;

    _oc1aPrevX = _oc1aStartX;
    _oc1aPrevY = _oc1aStartY;

    //повертати oc1aXDelta i oc1aYDelta ще залежно від wgm
    if(!_atmegaTimer.com1a0() && !_atmegaTimer.com1a1())
    {
        _oc1aYDelta = 0;
        _oc1aXDelta = 0;
        _oc1aPrevY = _oc1aStartY;
    }
    else if(_atmegaTimer.com1a0() && !_atmegaTimer.com1a1())
    {
        _oc1aYDelta = -20;
       _oc1aPrevY = _oc1aStartY;
    }
    else if(!_atmegaTimer.com1a0() && _atmegaTimer.com1a1())
    {
        //_oc1aYDelta = 20;
        _oc1aYDelta = 0;
        _oc1aPrevY = _oc1aStartY - 20;
    }
    else if(_atmegaTimer.com1a0() && _atmegaTimer.com1a1())
    {
        //_oc1aYDelta = -20;
        _oc1aYDelta = 0;
        _oc1aPrevY = _oc1aStartY;
    }

    _oc1aNewX = _oc1aPrevX;
    _oc1aNewY = _oc1aPrevY;

    //_oc1aYDelta = -20;
    //_oc1aXDelta = _xDelta;
}

void GraphicDrawer::buildCoordinates()
{
    buildCoordinateYMap();
    buildCoordinateXMap();

    /*buildCoordinateFixedYMap();
    buildCoordinateFixedXMap();*/
}

void GraphicDrawer::buildCoordinateYMap()
{
    //подумати над розширенням, бо для дуже малих значень таймера лінії нема або дуже мала
    //1. або будувати для кожного top своє співвідношення.
    //int yStep = _atmegaTimer.top() / _drawingSteps; //build from maximum value of timer
    qreal yStep = (qreal)getNeededTopValue()/*_atmegaTimer.top()*/ / (qreal)_drawingSteps;

    _stepsToReachMax = 65535 / yStep;

    //qInfo() << "YStep is: " << yStep << "\n";//int value = 0;
    qreal value = 0.0;
    for(int i = 0; i <= _drawingSteps; ++i)
    {
        _yCoordinateToBorder.insert(i, /*value*/ yStep * i);
        //value += yStep;

    }
    qInfo() << "last value in map is: " << _yCoordinateToBorder[_drawingSteps - 1] << "\n";
    //_yCoordinateToBorder.insert(_drawingSteps - 1, _atmegaTimer.top());

    qreal height = (_tcntStartY / 65535.0) * (qreal)getNeededTopValue()/*_atmegaTimer.top()*/;
    qreal prevDelta = _yDelta;
    _yDelta = height / (qreal)_drawingSteps; //must be timer max value - 65535
    if(!_isGoingUp)
    {
        _yDelta = -_yDelta;
    }
    qInfo() << "yDelta is: " << _yDelta << "\n";
    qreal diff = prevDelta / _yDelta;
    _performedSteps *= diff;
    //_performedSteps = 0;
}

void GraphicDrawer::buildCoordinateXMap()
{
    int actualSteps = getNeededTopValue();

    //add the calculation for other modes
    qreal secondsNeeded = (qreal)actualSteps/*_atmegaTimer.top()*/ / (qreal)_atmegaTimer.actualClk();
    qreal pixelsNeeded = _xDistanceBetweenPoints * secondsNeeded;
    _xDelta = pixelsNeeded / (qreal)_drawingSteps;
    if(_atmegaTimer.com1a0() || _atmegaTimer.com1a1())
    {
        _oc1aXDelta = _xDelta;
    }
    //_oc1aXDelta = _xDelta;
    qInfo() << "xDelta is: " << _xDelta << "\n";
}

void GraphicDrawer::buildCoordinateFixedYMap()
{
    _actualSteps = -1;
    qreal yStep = 65535.0 / (qreal)_drawingSteps;
    qreal currentTop = _atmegaTimer.top();
    qreal value = 0.0;
    _yCoordinateToBorder.insert(0, 0);
    for(int i = 1; i <= _drawingSteps; ++i)
    {
         value += yStep;
        _yCoordinateToBorder.insert(i, value);
        if(_actualSteps < 0 && qRound(currentTop) <= qRound(value))
        {
            qInfo() << "differ is: " << qAbs(currentTop - value) << "\n";//int value = 0;
            _actualSteps = i;
            //qInfo() << "actualSteps is: " << _actualSteps << "\n";//int value = 0;
        }
        qInfo() << "value is: " << qRound(value) << "\n";//int value = 0;
        qInfo() << "top is: " << qRound(currentTop) << "\n";//int value = 0;

    }
    if(_actualSteps < 0)
    {
       _actualSteps = 0;
    }
     qInfo() << "actualSteps is: " << _actualSteps << "\n";//int value = 0;
    //_yCoordinateToBorder.insert(_drawingSteps - 1, _atmegaTimer.top());

    qreal height = _tcntStartY;
    _yDelta = height / (qreal)_drawingSteps; //must be timer max value - 65535
    //_performedSteps = 0;
}

void GraphicDrawer::buildCoordinateFixedXMap()
{
    int actualSteps = getNeededTopValue();

    //add the calculation for other modes
    qreal secondsNeeded = (qreal)actualSteps/*_atmegaTimer.top()*/ / (qreal)_atmegaTimer.actualClk();
    qreal pixelsNeeded = _xDistanceBetweenPoints * secondsNeeded;
    _xDelta = pixelsNeeded / (qreal)_actualSteps;//int value = 0;
    _oc1aXDelta = _xDelta;

    // qreal secondsNeeded = (qreal)actualSteps/*_atmegaTimer.top()*/ / (qreal)_atmegaTimer.actualClk();
    //_xDelta = _xDistanceBetweenPoints / (qreal)_actualSteps;
}

 int GraphicDrawer::getNeededTopValue()
 {
     switch(_atmegaTimer.timerMode())
     {
         case WaveFormGenerator::Mode::Normal: return _atmegaTimer.top();

         case WaveFormGenerator::Mode::CTCOcr:
         case WaveFormGenerator::Mode::CTCIcr: return _atmegaTimer.top() + 1;

         case WaveFormGenerator::Mode::FastPWM8:
         case WaveFormGenerator::Mode::FastPWM9:
         case WaveFormGenerator::Mode::FastPWM10:
         case WaveFormGenerator::Mode::FastPWMOcr:
         case WaveFormGenerator::Mode::FastPWMIcr: return _atmegaTimer.top() + 1;

         case WaveFormGenerator::Mode::PWM8Ph:
         case WaveFormGenerator::Mode::PWM9Ph:
         case WaveFormGenerator::Mode::PWM10Ph:
         case WaveFormGenerator::Mode::PWMPhOcr:
         case WaveFormGenerator::Mode::PWMPhIcr:
         case WaveFormGenerator::Mode::PWMPhFrOcr:
         case WaveFormGenerator::Mode::PWMPhFrIcr: return _atmegaTimer.top();

         case WaveFormGenerator::Mode::Reserved: return 0;
     }
 }

 bool GraphicDrawer::equal(qreal left, qreal right)
 {
     const double dEpsilon = 0.000001;
     return qAbs(left - right) <= dEpsilon * qAbs(right);
 }

 bool GraphicDrawer::inRange(qreal val, qreal low, qreal high)
 {
     const double dEpsilon = 0.000001;
     return (val > low) && (val < high || equal(val, high));
 }
