#include "AtmegaTimer.h"

AtmegaTimer::AtmegaTimer(long t1, long inputFrequency, int icr1a, int ocr1a,
                         bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                         bool cs12, bool cs11, bool cs10):
                         _tcnt1(0),
                         _clockSelect(t1, inputFrequency, cs12, cs11, cs10),
                         _waveFormGenerator(ocr1a, icr1a, wgm13, wgm12, wgm11, wgm10) {}

AtmegaTimer::~AtmegaTimer() {}

int AtmegaTimer::top() const
{
    return _waveFormGenerator.top();
}

long AtmegaTimer::frequency() const
{
    return _clockSelect.frequency();
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

int AtmegaTimer::icr1a()
{
    return _waveFormGenerator.icr1a();
}

int AtmegaTimer::ocr1a()
{
    return _waveFormGenerator.ocr1a();
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

void AtmegaTimer::setCs10(bool newVal)
{
    _clockSelect.setCs10(newVal);
}

void AtmegaTimer::increase()
{
    ++_tcnt1;
}

void AtmegaTimer::decrease()
{
    --_tcnt1;
}
