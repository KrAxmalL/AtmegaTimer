#include "ClockSelect.h"

ClockSelect::ClockSelect(long t1, long frequency,
                         bool cs12, bool cs11, bool cs10): _t1(t1), _inputFrequency(frequency),
                                                           _cs12(cs12), _cs11(cs11), _cs10(cs10)
{
    _reg |= _cs10;
    _reg |= (_cs11 << 1);
    _reg |= (_cs12 << 2);

    state = static_cast<State>(_reg);

    _actualFrequency = calculateActualFrequency();
}

ClockSelect::~ClockSelect() {}

void ClockSelect::setCs10(bool newVal)
{
    _cs10 = newVal;
    _reg |= _cs10;
    state = static_cast<State>(_reg);
    _actualFrequency = calculateActualFrequency();
}

void ClockSelect::setCs11(bool newVal)
{
    _cs11 = newVal;
    _reg |= (_cs11 << 1);
    state = static_cast<State>(_reg);
    _actualFrequency = calculateActualFrequency();
}

void ClockSelect::setCs12(bool newVal)
{
    _cs12 = newVal;
    _reg |= (_cs12 << 2);
    state = static_cast<State>(_reg);
    _actualFrequency = calculateActualFrequency();
}

void ClockSelect::setFrequency(long frequency)
{
    _inputFrequency = frequency;
    _actualFrequency = calculateActualFrequency();
}

long ClockSelect::calculateActualFrequency()
{
    switch(state)
    {
        case State::Off: return 0;

        case State::Io1: return _inputFrequency;

        case State::Io8: return _inputFrequency / 8;

        case State::Io64: return _inputFrequency / 64;

        case State::Io256: return _inputFrequency / 256;

        case State::Io1024: return _inputFrequency / 1024;

        case State::ExT1F: return _t1;

        case State::ExT1R: return _t1;
    }
}
