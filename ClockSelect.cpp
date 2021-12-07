#include "ClockSelect.h"

ClockSelect::ClockSelect(long t1, long frequency,
                         bool cs12, bool cs11, bool cs10): _t1(t1), _clk(frequency), _actualClk(0),
                                                           _reg(0), _state(ClockSelect::State::Off),
                                                           _cs12(cs12), _cs11(cs11), _cs10(cs10)
{
    //set n's bit to val
    _reg = setBitNToX(_reg, 0, _cs10);
    _reg = setBitNToX(_reg, 1, _cs11);
    _reg = setBitNToX(_reg, 2, _cs12);

    _state = static_cast<State>(_reg);

    _actualClk = calculateActualFrequency();
}

ClockSelect::~ClockSelect() {}

void ClockSelect::setCs10(bool newVal)
{
    _cs10 = newVal;
   _reg = setBitNToX(_reg, 0, _cs10);
    _state = static_cast<State>(_reg);
    _actualClk = calculateActualFrequency();
}

void ClockSelect::setCs11(bool newVal)
{
    _cs11 = newVal;
    _reg = setBitNToX(_reg, 1, _cs11);
    _state = static_cast<State>(_reg);
    _actualClk = calculateActualFrequency();
}

void ClockSelect::setCs12(bool newVal)
{
    _cs12 = newVal;
    _reg = setBitNToX(_reg, 2, _cs12);
    _state = static_cast<State>(_reg);
    _actualClk = calculateActualFrequency();
}

void ClockSelect::setClk(long frequency)
{
    _clk = frequency;
    _actualClk = calculateActualFrequency();
}

void ClockSelect::setT1(long frequency)
{
    _t1 = frequency;
    _actualClk = calculateActualFrequency();
}

long ClockSelect::calculateActualFrequency()
{
    switch(_state)
    {
        case State::Off: return 0;

        case State::Io1: return _clk;

        case State::Io8: return _clk / 8;

        case State::Io64: return _clk / 64;

        case State::Io256: return _clk / 256;

        case State::Io1024: return _clk / 1024;

        case State::ExT1F: return _t1;

        case State::ExT1R: return _t1;
    }
}
