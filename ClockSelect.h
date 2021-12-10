#ifndef CLOCKSELECT_H
#define CLOCKSELECT_H

#include "BitUtils.h"

class ClockSelect
{
public:

    //only 3 bits needed
    enum class State: int
    {
        Off = 0x0,
        Io1 = 0x1,
        Io8 = 0x2,
        Io64 = 0x3,
        Io256 = 0x4,
        Io1024 = 0x5,
        ExT1F = 0x6,
        ExT1R = 0x7,
    };

    ClockSelect(long t1, long frequency, bool cs12, bool cs11, bool cs10);
    ~ClockSelect();

    inline bool cs10() const {return _cs10;}
    inline bool cs11() const {return _cs11;}
    inline bool cs12() const {return _cs12;}

    inline long t1() const {return _t1;}
    inline long clk() const {return _clk;}
    inline long actualClk() const {return _actualClk;}

    inline State state() const {return _state;}

    void setCs10(bool newVal);
    void setCs11(bool newVal);
    void setCs12(bool newVal);

    void setClk(long frequency);
    void setT1(long frequency);

private:

    bool _cs10;
    bool _cs11;
    bool _cs12;

    int _reg;

    int _t1;

    State _state;

    long _clk;
    long _actualClk;

    long calculateActualFrequency();
};

#endif // CLOCKSELECT_H
