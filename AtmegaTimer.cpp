#include "AtmegaTimer.h"

AtmegaTimer::AtmegaTimer(long t1, long inputFrequency, int icr1, int ocr1a, int ocr1b,
                         bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                         bool cs12, bool cs11, bool cs10, bool com1a1, bool com1a0, bool com1b1, bool com1b0):
                         _tcnt1(0), _tov1(false), _oc1a(false), _oc1b(false),
                         _waveFormGenerator(ocr1a, ocr1b, icr1, wgm13, wgm12, wgm11, wgm10),
                         _clockSelect(t1, inputFrequency, cs12, cs11, cs10),
                         _com1a1(com1a1), _com1a0(com1a0), _com1b1(com1b1), _com1b0(com1b0)
{
    int mode = 0;
    mode = setBitNToX(mode, 0, _com1a0);
    mode = setBitNToX(mode, 1, _com1a1);
    _outputModeA = static_cast<AtmegaTimer::OutputCompareMode>(mode);

    mode = 0;
    mode = setBitNToX(mode, 0, _com1b0);
    mode = setBitNToX(mode, 1, _com1b1);
    _outputModeB = static_cast<AtmegaTimer::OutputCompareMode>(mode);
}

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

int AtmegaTimer::ocr1b()
{
    return _waveFormGenerator.ocr1b();
}

int AtmegaTimer::ocr1aBuffer()
{
    return _waveFormGenerator.ocr1aBuffer();
}

int AtmegaTimer::ocr1bBuffer()
{
    return _waveFormGenerator.ocr1bBuffer();
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

void AtmegaTimer::setCom1a0(bool newVal)
{
    _com1a0 = newVal;

    int mode = static_cast<int>(_outputModeA);
    mode = setBitNToX(mode, 0, _com1a0);
    _outputModeA = static_cast<AtmegaTimer::OutputCompareMode>(mode);
}

void AtmegaTimer::setCom1a1(bool newVal)
{
    _com1a1 = newVal;

    int mode = static_cast<int>(_outputModeA);
    mode = setBitNToX(mode, 1, _com1a1);
    _outputModeA = static_cast<AtmegaTimer::OutputCompareMode>(mode);
}

void AtmegaTimer::setCom1b0(bool newVal)
{
    _com1b0 = newVal;

    int mode = static_cast<int>(_outputModeB);
    mode = setBitNToX(mode, 0, _com1b0);
    _outputModeB = static_cast<AtmegaTimer::OutputCompareMode>(mode);
}

void AtmegaTimer::setCom1b1(bool newVal)
{
    _com1b1 = newVal;

    int mode = static_cast<int>(_outputModeB);
    mode = setBitNToX(mode, 1, _com1b1);
    _outputModeB = static_cast<AtmegaTimer::OutputCompareMode>(mode);
}

void AtmegaTimer::setOc1a(bool newVal) { _oc1a = newVal; }

void AtmegaTimer::setOc1b(bool newVal) { _oc1b = newVal; }

void AtmegaTimer::setTov1(bool newVal) { _tov1 = newVal; }

void AtmegaTimer::setClk(long frequency) { _clockSelect.setClk(frequency); emit actualClkChanged(); }

void AtmegaTimer::setT1(long frequency) { _clockSelect.setT1(frequency); emit actualClkChanged(); }

void AtmegaTimer::setOcr1a(int val) { _waveFormGenerator.setOcr1aBuffer(val); }

void AtmegaTimer::setOcr1b(int val) { _waveFormGenerator.setOcr1bBuffer(val); }

void AtmegaTimer::loadOcr1aFromBuffer() {_waveFormGenerator.loadOcr1aFromBuffer(); emit topChanged();}

void AtmegaTimer::loadOcr1bFromBuffer() {_waveFormGenerator.loadOcr1bFromBuffer();}

void AtmegaTimer::setIcr1(int val) { _waveFormGenerator.setIcr1(val); emit topChanged(); }

int AtmegaTimer::comparableValueA()
{
    return _waveFormGenerator.comparableValueA();
}

int AtmegaTimer::comparableValueB()
{
    return _waveFormGenerator.comparableValueB();
}

WaveFormGenerator::Mode AtmegaTimer::timerMode()
{
    return _waveFormGenerator.mode();
}

ClockSelect::State AtmegaTimer::timerState()
{
    return _clockSelect.state();
}

AtmegaTimer::OutputCompareMode AtmegaTimer::outputModeA()
{
    return _outputModeA;
}

AtmegaTimer::OutputCompareMode AtmegaTimer::outputModeB()
{
    return _outputModeB;
}
