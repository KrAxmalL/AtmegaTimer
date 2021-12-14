#include "AtmegaTimer.h"

AtmegaTimer::AtmegaTimer(long t1, long inputFrequency, int icr1, int ocr1a,
                         bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                         bool cs12, bool cs11, bool cs10, bool com1a1, bool com1a0):
                         _tcnt1(0), _tov1(false), _oc1a(false),
                         _waveFormGenerator(ocr1a, icr1, wgm13, wgm12, wgm11, wgm10),
                         _clockSelect(t1, inputFrequency, cs12, cs11, cs10),
                         _com1a1(com1a1), _com1a0(com1a0) {}

AtmegaTimer::~AtmegaTimer() {}

int AtmegaTimer::top() const
{
    return _waveFormGenerator.top();
}

long AtmegaTimer::clk() const
{
    return _clockSelect.clk();
}

long AtmegaTimer::actualClk() const
{
    return _clockSelect.actualClk();
}

bool AtmegaTimer::cs10() const
{
    return _clockSelect.cs10();
}

bool AtmegaTimer::cs11() const
{
    return _clockSelect.cs11();
}

bool AtmegaTimer::cs12() const
{
    return _clockSelect.cs12();
}

long AtmegaTimer::t1()
{
    return _clockSelect.t1();
}

int AtmegaTimer::icr1()
{
    return _waveFormGenerator.icr1();
}

int AtmegaTimer::ocr1a()
{
    return _waveFormGenerator.ocr1a();
}

int AtmegaTimer::ocr1aBuffer()
{
    return _waveFormGenerator.ocr1aBuffer();
}

bool AtmegaTimer::wgm10() const
{
    return _waveFormGenerator.wgm10();
}

bool AtmegaTimer::wgm11() const
{
    return _waveFormGenerator.wgm11();
}

bool AtmegaTimer::wgm12() const
{
    return _waveFormGenerator.wgm12();
}

bool AtmegaTimer::wgm13() const
{
    return _waveFormGenerator.wgm13();
}

void AtmegaTimer::setCs10(bool newVal) { _clockSelect.setCs10(newVal); emit actualClkChanged(); }

void AtmegaTimer::setCs11(bool newVal) { _clockSelect.setCs11(newVal); emit actualClkChanged(); }

void AtmegaTimer::setCs12(bool newVal) { _clockSelect.setCs12(newVal); emit actualClkChanged(); }

void AtmegaTimer::setWgm10(bool newVal) { _waveFormGenerator.setWgm10(newVal); emit topChanged(); }

void AtmegaTimer::setWgm11(bool newVal) { _waveFormGenerator.setWgm11(newVal); emit topChanged(); }

void AtmegaTimer::setWgm12(bool newVal) { _waveFormGenerator.setWgm12(newVal); emit topChanged(); }

void AtmegaTimer::setWgm13(bool newVal) { _waveFormGenerator.setWgm13(newVal); emit topChanged(); }

void AtmegaTimer::setCom1a0(bool newVal) { _com1a0 = newVal; }

void AtmegaTimer::setCom1a1(bool newVal) { _com1a1 = newVal; }

void AtmegaTimer::setOc1a(bool newVal) { _oc1a = newVal; }

void AtmegaTimer::setTov1(bool newVal) { _tov1 = newVal; }

void AtmegaTimer::setClk(long frequency) { _clockSelect.setClk(frequency); emit actualClkChanged(); }

void AtmegaTimer::setT1(long frequency) { _clockSelect.setT1(frequency); emit actualClkChanged(); }

void AtmegaTimer::setOcr1a(int val) { _waveFormGenerator.setOcr1aBuffer(val); }

void AtmegaTimer::loadOcr1aFromBuffer() {_waveFormGenerator.loadOcr1aFromBuffer(); emit topChanged();}

void AtmegaTimer::setIcr1(int val) { _waveFormGenerator.setIcr1(val); emit topChanged(); }

void AtmegaTimer::increase()
{
    ++_tcnt1;
}

void AtmegaTimer::decrease()
{
    --_tcnt1;
}

void AtmegaTimer::performClock()
{
    switch(_waveFormGenerator.mode())
    {
        case WaveFormGenerator::Mode::Normal: normalStep(); return;

        case WaveFormGenerator::Mode::CTCOcr:
        case WaveFormGenerator::Mode::CTCIcr: ctcStep(); return;

        case WaveFormGenerator::Mode::FastPWM8:
        case WaveFormGenerator::Mode::FastPWM9:
        case WaveFormGenerator::Mode::FastPWM10:
        case WaveFormGenerator::Mode::FastPWMOcr:
        case WaveFormGenerator::Mode::FastPWMIcr: fastPwmStep(); return;

        case WaveFormGenerator::Mode::PWM8Ph:
        case WaveFormGenerator::Mode::PWM9Ph:
        case WaveFormGenerator::Mode::PWM10Ph:
        case WaveFormGenerator::Mode::PWMPhOcr:
        case WaveFormGenerator::Mode::PWMPhIcr: phaseCorrectStep(); return;

        case WaveFormGenerator::Mode::PWMPhFrOcr:
        case WaveFormGenerator::Mode::PWMPhFrIcr: phaseAndFrequencyCorrectStep(); return;

        case WaveFormGenerator::Mode::Reserved: return;
    }
}

int AtmegaTimer::comparableValue()
{
    return _waveFormGenerator.comparableValue();
}

WaveFormGenerator::Mode AtmegaTimer::timerMode()
{
    return _waveFormGenerator.mode();
}

ClockSelect::State AtmegaTimer::timerState()
{
    return _clockSelect.state();
}

void AtmegaTimer::normalStep()
{
    if(_tcnt1 == _waveFormGenerator.top())
    //if(_tcnt1 == 65535)
    {
        _tcnt1 = 0;
        _tov1 = 1;
        //depends on com1 mode
        _oc1a = !_oc1a;

        emit tov1Changed();
        emit oc1aChanged();
    }
    else
    {
        ++_tcnt1;
        //increase();
    }
}

void AtmegaTimer::ctcStep()
{
    if(_tcnt1 == _waveFormGenerator.top())
    {
        _tcnt1 = 0;
        _tov1 = 1; //maybe not needed
        //depends on com1 mode
        _oc1a = !_oc1a;

        emit tov1Changed();
        emit oc1aChanged();
    }
    else
    {
        ++_tcnt1;
        //increase();
    }
}

void AtmegaTimer::fastPwmStep()
{
    if(_tcnt1 == _waveFormGenerator.ocr1a())
    {
        //depends on com1 mode
        _oc1a = !_oc1a;

        emit oc1aChanged();
    }

    if(_tcnt1 == _waveFormGenerator.top())
    {
        _tcnt1 = 0;
        _tov1 = 1;
        //depends on com1 mode
        _oc1a = !_oc1a;

        emit tov1Changed();
        emit oc1aChanged();
    }
    else
    {
        ++_tcnt1;
    }
}

void AtmegaTimer::phaseCorrectStep()
{

}

void AtmegaTimer::phaseAndFrequencyCorrectStep()
{

}
