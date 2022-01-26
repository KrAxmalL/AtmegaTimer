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

void GraphicDrawer::drawNextCtcMode()
{
    int prevOcr1a = _atmegaTimer.ocr1a();
    _atmegaTimer.loadOcr1aFromBuffer();
    if(prevOcr1a != _atmegaTimer.ocr1a())
    {
        updateCoordinates();
    }

    _atmegaTimer.loadOcr1bFromBuffer();

    if(_reachedTop)
    {
       dropTcntToBottom();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        if(_performedSteps == _stepsToReachMax)
        {
            _reachedTop = true;
        }

        if(_performedSteps <= _drawingSteps)
        {
            bool zeroTop = equal(getNeededTopValue(), 0);

            if(zeroTop)
            {
                if(_performedSteps == _drawingSteps)
                {
                    _reachedTop = true;
                }

                bool changeNeeded = (_stepsForDivision > 0) && ((_stepsForDivision % _stepsForZeroCtc) == 0);
                if(changeNeeded)
                {
                    updateOc1a();

                }

                if(!_reachedTop && _performedSteps > 0)
                {
                    ++_stepsForDivision;
                }

            }
            else
            {
                bool reachedA = false;
                bool reachedB = false;

                if(_performedSteps == 0)
                {
                    reachedA = _atmegaTimer.ocr1a() <=  _yCoordinateToBorder[_performedSteps];
                    reachedB = _atmegaTimer.ocr1b() <=  _yCoordinateToBorder[_performedSteps];
                }
                else
                {
                    reachedA = inRange(_atmegaTimer.ocr1a(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                    reachedB = inRange(_atmegaTimer.ocr1b(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                }

                if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
                {
                    _reachedTop = true;
                }

                if(reachedA)
                {
                    updateOc1a();
                }

                if(reachedB)
                {
                    updateOc1b();
                }
            }
        }

        ++_performedSteps;
    }

    _scene.addLine(QLine(_tcntPrevX, _tcntPrevY, _tcntNewX, _tcntNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1aPrevX, _oc1aPrevY, _oc1aNewX, _oc1aNewY), QPen(Qt::GlobalColor::black));
    _scene.addLine(QLine(_oc1bPrevX, _oc1bPrevY, _oc1bNewX, _oc1bNewY), QPen(Qt::GlobalColor::black));
}

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

        if(_stepsToReachMax == _performedSteps)
        {
            _reachedTop = true;
        }

        if(_performedSteps <= _drawingSteps)
        {
            bool equalA = false;
            bool equalB = false;

            if(_performedSteps == 0)
            {
                int prevOcr1a = _atmegaTimer.ocr1a();
                _atmegaTimer.loadOcr1aFromBuffer();
                if(prevOcr1a != _atmegaTimer.ocr1a())
                {
                    updateCoordinates();
                }

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
            }

            if(equalB)
            {
               updateOc1b();
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
            {
                _reachedTop = true;

                if(_atmegaTimer.outputModeA() != AtmegaTimer::OutputCompareMode::Toggle)
                {
                    updateOc1a();
                }
                updateOc1b();
                updateTov1();
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

        int prevOcr1a = _atmegaTimer.ocr1a();
        _atmegaTimer.loadOcr1aFromBuffer();
        if(prevOcr1a != _atmegaTimer.ocr1a())
        {
            updateCoordinates();
        }
        _atmegaTimer.loadOcr1bFromBuffer();
    }
    else
    {
        updateTcntCoordinates();
        updateOc1aCoordinates();
        updateOc1bCoordinates();

        if(_stepsToReachMax == _performedSteps && _isGoingUp)
        {
            _reachedTop = true;
        }

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

            if(equalA)
            {
                updateOc1a();
            }

            if(equalB)
            {
                updateOc1b();
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
            {
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

        if(_stepsToReachMax == _performedSteps && _isGoingUp)
        {
            _reachedTop = true;
        }

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

                    int prevOcr1a = _atmegaTimer.ocr1a();
                    _atmegaTimer.loadOcr1aFromBuffer();
                    if(prevOcr1a != _atmegaTimer.ocr1a())
                    {
                        updateCoordinates();
                    }
                    _atmegaTimer.loadOcr1bFromBuffer();
                }
            }
            else
            {
                equalA = inRange(_atmegaTimer.comparableValueA(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
                equalB = inRange(_atmegaTimer.comparableValueB(), _yCoordinateToBorder[_performedSteps - 1], _yCoordinateToBorder[_performedSteps]);
            }

            if(equalA)
            {
                updateOc1a();
            }

            if(equalB)
            {
                updateOc1b();
            }

            if(equal(getNeededTopValue(), _yCoordinateToBorder[_performedSteps]))
            {
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
    _stepsForZeroCtc = 0;
    _stepsForDivision = 0;

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
    }
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
    }
}

void GraphicDrawer::refreshDeltasA()
{
    if(_atmegaTimer.outputModeA() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
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
            case WaveFormGenerator::Mode::PWMPhFrOcr: _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::PWMPhIcr:
            case WaveFormGenerator::Mode::PWMPhFrIcr:
            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::FastPWM8:
            case WaveFormGenerator::Mode::FastPWM9:
            case WaveFormGenerator::Mode::FastPWM10:
            case WaveFormGenerator::Mode::PWM8Ph:
            case WaveFormGenerator::Mode::PWM9Ph:
            case WaveFormGenerator::Mode::PWM10Ph:
            case WaveFormGenerator::Mode::Reserved: _oc1aXDelta = 0; break;
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
            case WaveFormGenerator::Mode::PWMPhFrOcr: _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _oc1aXDelta = 0; break;
        }
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
            case WaveFormGenerator::Mode::PWMPhFrOcr: _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _oc1aXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _oc1aXDelta = 0; break;
        }
    }
}

void GraphicDrawer::refreshDeltasB()
{
    if(_atmegaTimer.outputModeB() == AtmegaTimer::OutputCompareMode::Disconnected)
    {
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
            case WaveFormGenerator::Mode::Reserved: _oc1bXDelta = 0; break;
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
            case WaveFormGenerator::Mode::PWMPhFrOcr: _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::CTCOcr:
            case WaveFormGenerator::Mode::CTCIcr: _oc1bXDelta = _xDelta; break;

            case WaveFormGenerator::Mode::Normal:
            case WaveFormGenerator::Mode::Reserved: _oc1bXDelta = 0; break;
        }
    }
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
    qreal yStep = (qreal)getNeededTopValue() / (qreal)_drawingSteps;

    if(getNeededTopValue() == 0)
    {
        _stepsToReachMax = _drawingSteps;
    }
    else
    {
       _stepsToReachMax = 65535 / yStep;
    }

    qreal value = 0.0;
    for(int i = 0; i <= _drawingSteps; ++i)
    {
        _yCoordinateToBorder.insert(i, yStep * i);

    }

    qreal height = (_tcntStartY / 65535.0) * (qreal)getNeededTopValue();
    qreal prevDelta = _yDelta;
    _yDelta = height / (qreal)_drawingSteps;
    if(!_isGoingUp)
    {
        _yDelta = -_yDelta;
    }

    qreal diff = 1;
    if(!equal(prevDelta, _yDelta))
    {
        diff = prevDelta / _yDelta;
    }
    _performedSteps *= diff;
}

void GraphicDrawer::buildCoordinateXMap()
{
    qreal pixelsNeeded = 0;
    int actualSteps = getNeededTopValue();
    if(actualSteps == 0)
    {
        pixelsNeeded = _xDistanceBetweenPoints;
        if(_atmegaTimer.actualClk() == 0)
        {
             _stepsForZeroCtc = 0;
        }
        else
        {
            _stepsForZeroCtc = 2 * (_drawingSteps / _atmegaTimer.actualClk());
        }
    }
    else
    {
        qreal secondsNeeded = (qreal)actualSteps/ (qreal)_atmegaTimer.actualClk();
        pixelsNeeded = _xDistanceBetweenPoints * secondsNeeded;
    }

    _xDelta = pixelsNeeded / (qreal)_drawingSteps;
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
