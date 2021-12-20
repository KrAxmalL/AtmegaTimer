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

void GraphicDrawer::drawNextNormalMode()
{
    _atmegaTimer.loadOcr1aFromBuffer();
    _atmegaTimer.loadOcr1bFromBuffer();

    if(_reachedTop)
    {
       dropTcntToBottom();
    }
    else
    {
        updateTcntCoordinates();

        if(equal(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps]))
        {
            _reachedTop = true;

            updateTov1();
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
}

//закінчити
//але додатково перевірити на малих та крайнії значеннях
void GraphicDrawer::drawNextCtcMode()
{
    int prevOcr1a = _atmegaTimer.ocr1a();
    _atmegaTimer.loadOcr1aFromBuffer();
    if(prevOcr1a != _atmegaTimer.ocr1a())
    {
        updateCoordinates();
    }

    //int prevOcr1b = _atmegaTimer.ocr1b();
    _atmegaTimer.loadOcr1bFromBuffer();
    /*if(prevOcr1b != _atmegaTimer.ocr1b())
    {
        updateCoordinates();
    }*/

    if(_reachedTop)
    {
       dropTcntToBottom();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        bool reachedA = false;
        bool reachedB = false;

        if(_performedSteps == _stepsToReachMax) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
        }

        if(_performedSteps <= _drawingSteps)
        {
            //replace comparableValue with register value
            if(_performedSteps == 0)
            {
                reachedA = _atmegaTimer.comparableValueA() <=  _yCoordinateToBorder[_performedSteps];
                reachedB = _atmegaTimer.comparableValueB() <=  _yCoordinateToBorder[_performedSteps];
            }
            else
            {
                reachedA = inRange(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                reachedB = inRange(_atmegaTimer.comparableValueB(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
            {
                _reachedTop = true;
            }

            if(reachedA/*equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                updateOc1a();
            }

            if(reachedB)
            {
                updateOc1b();
            }
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    qInfo() << "newY is: " << (_tcntNewY - _yDelta) << "\n";
    qInfo() << "drawed line is: " << _tcntPrevX << " " << _tcntPrevY << " " << _tcntNewX << " " << _tcntNewY << "\n";
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1bPrevX, _oc1bPrevY, _oc1bNewX, _oc1bNewY), QPen(Qt::GlobalColor::black));
}

//зробити малювання при 0 по-іншому
void GraphicDrawer::drawNextFastPwmMode()
{
    if(_reachedTop)
    {
       dropTcntToBottom();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        if(_stepsToReachMax == _performedSteps) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool equalA = false;
            bool equalB = false;

            if(_performedSteps == 0)
            {
                _atmegaTimer.loadOcr1aFromBuffer();
                _atmegaTimer.loadOcr1bFromBuffer();

                equalA = _atmegaTimer.comparableValueA() <=  _yCoordinateToBorder[_performedSteps];
                equalB = _atmegaTimer.comparableValueB() <=  _yCoordinateToBorder[_performedSteps];
            }
            else
            {
                equalA = inRange(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                equalB = inRange(_atmegaTimer.comparableValueB(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(equalA)
            {
               updateOc1a();

                qInfo() << "comparable value A is: " << (qreal)_atmegaTimer.comparableValueA() << "\n";
            }

            if(equalB)
            {
               updateOc1b();

                qInfo() << "comparable value B is: " << (qreal)_atmegaTimer.comparableValueB() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {
                _reachedTop = true;

                if(_atmegaTimer.outputModeA() != AtmegaTimer::OutputCompareMode::Toggle)
                {
                    updateOc1a();
                }
                updateOc1b();
                updateTov1();

                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";
                qInfo() << "value in map: " << (qreal)_yCoordinateToBorder[_performedSteps] << "\n";
            }
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1bPrevX, _oc1bPrevY, _oc1bNewX, _oc1bNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextPhaseCorrectPwmMode()
{
    if(_reachedTop)
    {
        setTcntDecreasing();
        _atmegaTimer.loadOcr1aFromBuffer();
        _atmegaTimer.loadOcr1bFromBuffer();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        if(_stepsToReachMax == _performedSteps && _isGoingUp) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
            qInfo() << "reached top through steps: " << _performedSteps << "\n";
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool equalA = false;
            bool equalB = false;

            if(_performedSteps == 0)
            {
                if(!_isGoingUp)
                {
                    updateTov1();

                    _yDelta = -_yDelta;
                    _isGoingUp = true;
                }
            }
            else
            {
                equalA = inRange(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                equalB = inRange(_atmegaTimer.comparableValueB(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(equalA/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                updateOc1a();

                qInfo() << "comparable value A is: " << (qreal)_atmegaTimer.comparableValueA() << "\n";
            }

            if(equalB/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                updateOc1b();

                qInfo() << "comparable value B is: " << (qreal)_atmegaTimer.comparableValueB() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {

                qInfo() << "reached top in phase correct with steps: " << _performedSteps << "\n";
                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";

                if(_isGoingUp)
                {
                    _reachedTop = true;
                }

                if(equalA && _atmegaTimer.outputModeA() != AtmegaTimer::OutputCompareMode::Toggle)
                {
                    updateOc1a();
                }

                if(equalB)
                {
                    updateOc1b();
                }
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
    _scene.addLine(QLine(_oc1bPrevX, _oc1bPrevY, _oc1bNewX, _oc1bNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::drawNextPhaseFrequencyCorrectMode()
{
    if(_reachedTop)
    {
        setTcntDecreasing();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        if(_stepsToReachMax == _performedSteps && _isGoingUp) //add check that reached max possible value - 65535
        {
            _reachedTop = true;
            qInfo() << "reached top through steps: " << _performedSteps << "\n";
        }

        //replace all checks with range
        if(_performedSteps <= _drawingSteps)
        {
            bool equalA = false;
            bool equalB = false;

            if(_performedSteps == 0)
            {
                if(!_isGoingUp)
                {
                    updateTov1();

                    _yDelta = -_yDelta;
                    _isGoingUp = true;

                    _atmegaTimer.loadOcr1aFromBuffer();
                    _atmegaTimer.loadOcr1bFromBuffer();
                }
            }
            else
            {
                equalA = inRange(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                equalB = inRange(_atmegaTimer.comparableValueB(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(equalA/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                updateOc1a();

                qInfo() << "comparable value is: " << (qreal)_atmegaTimer.comparableValueA() << "\n";
            }

            if(equalB/*equal(_atmegaTimer.comparableValue(), _yCoordinateToBorder[_performedSteps])*/)
            {
                updateOc1b();

                qInfo() << "comparable value B is: " << (qreal)_atmegaTimer.comparableValueB() << "\n";
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps])/*(qreal)getNeededTopValue() <= _yCoordinateToBorder[_performedSteps]*/)
            {

                qInfo() << "reached top in phase correct with steps: " << _performedSteps << "\n";
                qInfo() << "needed top value is: " << (qreal)getNeededTopValue() << "\n";

                if(_isGoingUp)
                {
                    _reachedTop = true;
                }

                if(equalA && _atmegaTimer.outputModeA() != AtmegaTimer::OutputCompareMode::Toggle)
                {
                    updateOc1a();
                }

                if(equalB)
                {
                    updateOc1b();
                }
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
    _scene.addLine(QLine(_oc1bPrevX, _oc1bPrevY, _oc1bNewX, _oc1bNewY), QPen(Qt::GlobalColor::black));
}

void GraphicDrawer::updateTcntCoordinates()
{
    _tcntPrevX = _tcntNewX;
    _tcntPrevY = _tcntNewY;

    _tcntNewX += _xDelta;
    _tcntNewY -= _yDelta;
}

void GraphicDrawer::updateOc1aCoordinates()
{
    _oc1aPrevX = _oc1aNewX;
    _oc1aPrevY = _oc1aNewY;

    _oc1aNewX += _oc1aXDelta;
    _oc1aNewY = _oc1aPrevY;
}

void GraphicDrawer::updateOc1bCoordinates()
{
    _oc1bPrevX = _oc1bNewX;
    _oc1bPrevY = _oc1bNewY;

    _oc1bNewX += _oc1bXDelta;
    _oc1bNewY = _oc1bPrevY;
}

void GraphicDrawer::updateOc1a()
{
    _atmegaTimer.setOc1a(!_atmegaTimer.oc1a());
    emit _atmegaTimer.oc1aChanged();

    _oc1aNewY += _oc1aYDelta;
    _oc1aYDelta = -_oc1aYDelta;
    _oc1aNewX = _oc1aPrevX;
}

void GraphicDrawer::updateOc1b()
{
    _atmegaTimer.setOc1b(!_atmegaTimer.oc1b());
    emit _atmegaTimer.oc1bChanged();

    _oc1bNewY += _oc1bYDelta;
    _oc1bYDelta = -_oc1bYDelta;
    _oc1bNewX = _oc1bPrevX;
}

void GraphicDrawer::updateTov1()
{
    _atmegaTimer.setTov1(true);
    emit _atmegaTimer.tov1Changed();
}

void GraphicDrawer::dropTcntToBottom()
{
    _tcntNewX = _tcntPrevX;
    _tcntNewY = _tcntStartY;

    _performedSteps = 0;
    _reachedTop = false;
}

void GraphicDrawer::setTcntDecreasing()
{
    _tcntNewX = _tcntPrevX;
    _tcntNewY = _tcntPrevY;

    _isGoingUp = false;
    _reachedTop = false;

    _yDelta = -_yDelta;
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
        _scene.addLine(_ox0 + (i*_xDistanceBetweenPoints), _oy0 + 5, _ox0 + (i*_xDistanceBetweenPoints), _oy0 - 5);
    }

    QGraphicsTextItem* tcntText = _scene.addText("TCNT1");
    tcntText->setPos(_tcntStartX - 50, _tcntStartY - 10);

    QGraphicsTextItem* oc1bText = _scene.addText("OC1B");
    oc1bText->setPos(_oc1bStartX - 50, _oc1bStartY - 10);

    QGraphicsTextItem* oc1aText = _scene.addText("OC1A");
    oc1aText->setPos(_oc1aStartX - 50, _oc1aStartY - 10);

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
    _oc1aNewX = _oc1aPrevX;
    _oc1aNewY = _oc1aPrevY;

    _oc1bPrevX = _oc1bStartX;
    _oc1bPrevY = _oc1bStartY;
    _oc1bNewX = _oc1bPrevX;
    _oc1bNewY = _oc1bPrevY;

    //determineDeltas();
}

void GraphicDrawer::determineDeltasA()
{
    if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
        _atmegaTimer.setOc1a(false);
        emit _atmegaTimer.oc1aChanged();

        _oc1aYDelta = 0;
        _oc1aXDelta = 0;
        _oc1aPrevY = _oc1aStartY;
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Toggle)
    {
        _atmegaTimer.setOc1a(false);
        emit _atmegaTimer.oc1aChanged();

        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr:
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: _oc1aYDelta = -20; _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::Reserved: _oc1aYDelta = 0; _oc1aXDelta = 0; break;
        }

       _oc1aPrevY = _oc1aStartY;
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Normal)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10: _atmegaTimer.setOc1a(true); _oc1aYDelta = 20; _oc1aXDelta = _xDelta; _oc1aPrevY = _oc1aStartY - 20; break;

            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            {
                if(_atmegaTimer.ocr1a() == 0)
                {
                    _atmegaTimer.setOc1a(false);
                    _oc1aYDelta = -20;
                    _oc1aPrevY = _oc1aStartY;
                }
                else if(_atmegaTimer.ocr1a() == _atmegaTimer.top())
                {
                    _atmegaTimer.setOc1a(true);
                    _oc1aYDelta = 20;
                    _oc1aPrevY = _oc1aStartY - 20;
                }
                else
                {
                    _atmegaTimer.setOc1a(true);
                    _oc1aYDelta = 20;
                    _oc1aPrevY = _oc1aStartY - 20;
                }
                _oc1aXDelta = _xDelta;
                break;
            }

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _atmegaTimer.setOc1a(true); _oc1aYDelta = 0; _oc1aXDelta = _xDelta; _oc1aPrevY = _oc1aStartY - 20; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _atmegaTimer.setOc1a(false); _oc1aYDelta = 0; _oc1aXDelta = 0; _oc1aPrevY = _oc1aStartY; break;
        }

         emit _atmegaTimer.oc1aChanged();

        //_oc1aYDelta = 20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY - 20;
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Inverted)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10: _atmegaTimer.setOc1a(false); _oc1aYDelta = -20; _oc1aXDelta = _xDelta; _oc1aPrevY = _oc1aStartY; break;

            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            {
                if(_atmegaTimer.ocr1a() == 0)
                {
                    _atmegaTimer.setOc1a(true);
                    _oc1aYDelta = 20;
                    _oc1aPrevY = _oc1aStartY - 20;
                    break;
                }
                else if(_atmegaTimer.ocr1a() == _atmegaTimer.top())
                {
                    _atmegaTimer.setOc1a(false);
                    _oc1aYDelta = -20;
                    _oc1aPrevY = _oc1aStartY;
                    break;
                }
                else
                {
                    _atmegaTimer.setOc1a(false);
                    _oc1aYDelta = -20;
                    _oc1aPrevY = _oc1aStartY;
                }
                _oc1aXDelta = _xDelta;
                break;
            }

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr:  _atmegaTimer.setOc1a(false); _oc1aYDelta = 0; _oc1aXDelta = _xDelta; _oc1aPrevY = _oc1aStartY; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _atmegaTimer.setOc1a(false); _oc1aYDelta = 0; _oc1aXDelta = 0; _oc1aPrevY = _oc1aStartY; break;
        }

        emit _atmegaTimer.oc1aChanged();
        //_oc1aYDelta = -20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY;
    }

    //_oc1aYDelta = -20;
    //_oc1aXDelta = _xDelta;
}

void GraphicDrawer::determineDeltasB()
{
    if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
        _atmegaTimer.setOc1b(false);
        emit _atmegaTimer.oc1bChanged();

        _oc1bYDelta = 0;
        _oc1bXDelta = 0;
        _oc1bPrevY = _oc1bStartY;
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Toggle)
    {
        _atmegaTimer.setOc1b(false);
        emit _atmegaTimer.oc1bChanged();

        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _oc1bYDelta = -20; _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::Reserved: _oc1bYDelta = 0; _oc1bXDelta = 0; break;
        }

       _oc1bPrevY = _oc1bStartY;
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Normal)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10: _atmegaTimer.setOc1b(true); _oc1bYDelta = 20; _oc1bXDelta = _xDelta; _oc1bPrevY = _oc1bStartY - 20; break;

            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            {
                if(_atmegaTimer.ocr1b() == 0)
                {
                    _atmegaTimer.setOc1b(false);
                    _oc1bYDelta = -20;
                    _oc1bPrevY = _oc1bStartY;
                }
                else if(_atmegaTimer.ocr1b() == _atmegaTimer.top())
                {
                    _atmegaTimer.setOc1b(true);
                    _oc1bYDelta = 20;
                    _oc1bPrevY = _oc1bStartY - 20;
                }
                else
                {
                    _atmegaTimer.setOc1b(true);
                    _oc1bYDelta = 20;
                    _oc1bPrevY = _oc1bStartY - 20;
                }
                _oc1bXDelta = _xDelta;
                break;
            }

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _atmegaTimer.setOc1b(true); _oc1bYDelta = 0; _oc1bXDelta = _xDelta; _oc1bPrevY = _oc1bStartY - 20; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _atmegaTimer.setOc1b(false); _oc1bYDelta = 0; _oc1bXDelta = 0; _oc1bPrevY = _oc1bStartY; break;
        }

         emit _atmegaTimer.oc1bChanged();

        //_oc1aYDelta = 20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY - 20;
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Inverted)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10: _atmegaTimer.setOc1b(false); _oc1bYDelta = -20; _oc1bXDelta = _xDelta; _oc1bPrevY = _oc1bStartY; break;

            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            {
                if(_atmegaTimer.ocr1b() == 0)
                {
                    _atmegaTimer.setOc1b(true);
                    _oc1bYDelta = 20;
                    _oc1bPrevY = _oc1bStartY - 20;
                    break;
                }
                else if(_atmegaTimer.ocr1b() == _atmegaTimer.top())
                {
                    _atmegaTimer.setOc1b(false);
                    _oc1bYDelta = -20;
                    _oc1bPrevY = _oc1bStartY;
                    break;
                }
                else
                {
                    _atmegaTimer.setOc1b(false);
                    _oc1bYDelta = -20;
                    _oc1bPrevY = _oc1bStartY;
                }
                _oc1bXDelta = _xDelta;
                break;
            }

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr:  _atmegaTimer.setOc1b(false); _oc1bYDelta = 0; _oc1bXDelta = _xDelta; _oc1bPrevY = _oc1bStartY; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _atmegaTimer.setOc1b(false); _oc1bYDelta = 0; _oc1bXDelta = 0; _oc1bPrevY = _oc1bStartY; break;
        }

        emit _atmegaTimer.oc1bChanged();
        //_oc1aYDelta = -20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY;
    }

    //_oc1aYDelta = -20;
    //_oc1aXDelta = _xDelta;
}

void GraphicDrawer::refreshDeltasA()
{
    if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
        /*_oc1aYDelta = 0;*/
        _oc1aXDelta = 0;
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Toggle)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr:
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: /*_oc1aYDelta = -20;*/ _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1aXDelta = 0; break;
        }
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Normal)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: /*_oc1aYDelta = 20;*/ _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: /*_oc1aYDelta = 0;*/ _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1aXDelta = 0; break;
        }
        //_oc1aYDelta = 20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY - 20;
    }
    else if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Inverted)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: /*_oc1aYDelta = -20;*/ _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: /*_oc1aYDelta = 0;*/ _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1aXDelta = 0; break;
        }
        //_oc1aYDelta = -20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY;
    }

    //_oc1aYDelta = -20;
    //_oc1aXDelta = _xDelta;
}

void GraphicDrawer::refreshDeltasB()
{
    if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
        /*_oc1aYDelta = 0;*/
        _oc1bXDelta = 0;
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Toggle)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1bXDelta = 0; break;
        }
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Normal)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: /*_oc1aYDelta = 20;*/ _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: /*_oc1aYDelta = 0;*/ _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1bXDelta = 0; break;
        }
        //_oc1aYDelta = 20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY - 20;
    }
    else if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Inverted)
    {
        switch(_atmegaTimer.timerMode())
        {
            case WaveFormGenerator::Mode::FastPWMOcr:
            case WaveFormGenerator::Mode::FastPWMIcr:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::PWMPhOcr:
            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::PWMPhFrOcr: /*_oc1aYDelta = -20;*/ _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: /*_oc1aYDelta = 0;*/ _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: /*_oc1aYDelta = 0;*/ _oc1bXDelta = 0; break;
        }
        //_oc1aYDelta = -20;
        //_oc1aYDelta = 0;
        //_oc1aXDelta = _xDelta;
        //_oc1aPrevY = _oc1aStartY;
    }

    //_oc1aYDelta = -20;
    //_oc1aXDelta = _xDelta;
}

void GraphicDrawer::buildCoordinates()
{
    buildCoordinateYMap();
    buildCoordinateXMap();

    determineDeltasA();
    _oc1aNewX = _oc1aPrevX;
    _oc1aNewY = _oc1aPrevY;

    determineDeltasB();
    _oc1bNewX = _oc1bPrevX;
    _oc1bNewY = _oc1bPrevY;
}

void GraphicDrawer::updateCoordinates()
{
    buildCoordinateYMap();
    buildCoordinateXMap();

    refreshDeltasA();
    refreshDeltasB();
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
    //_oc1aXDelta = _xDelta;

    qInfo() << "xDelta is: " << _xDelta << "\n";
}

 int GraphicDrawer::getNeededTopValue()
 {
     switch(_atmegaTimer.timerMode())
     {
         case WaveFormGenerator::Mode::CTCOcr:
         case WaveFormGenerator::Mode::CTCIcr:
         case WaveFormGenerator::Mode::Normal:
         case WaveFormGenerator::Mode::PWM8Ph:
         case WaveFormGenerator::Mode::PWM9Ph:
         case WaveFormGenerator::Mode::PWM10Ph:
         case WaveFormGenerator::Mode::PWMPhOcr:
         case WaveFormGenerator::Mode::PWMPhIcr:
         case WaveFormGenerator::Mode::PWMPhFrOcr:
         case WaveFormGenerator::Mode::PWMPhFrIcr: return _atmegaTimer.top();

         //case WaveFormGenerator::Mode::CTCOcr:
         //case WaveFormGenerator::Mode::CTCIcr:
         case WaveFormGenerator::Mode::FastPWM8:
         case WaveFormGenerator::Mode::FastPWM9:
         case WaveFormGenerator::Mode::FastPWM10:
         case WaveFormGenerator::Mode::FastPWMOcr:
         case WaveFormGenerator::Mode::FastPWMIcr: return _atmegaTimer.top() + 1;

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
