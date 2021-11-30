#ifndef ATMEGATIMER_H
#define ATMEGATIMER_H

#include "WaveFormGenerator.h"
#include "ClockSelect.h"

class AtmegaTimer
{

public:

    AtmegaTimer(long t1, long inputFrequency, int icr1a, int ocr1a,
                bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                bool cs12, bool cs11, bool cs10);

    ~AtmegaTimer();

    inline int tcnt1() const {return _tcnt1;}

    int top() const;

    long t1();
    long frequency() const;

    bool cs10() const;
    bool cs11() const;
    bool cs12() const;

    int icr1a();

    int ocr1a();

    bool wgm10() const;
    bool wgm11() const;
    bool wgm12() const;
    bool wgm13() const;

    void increase();
    void decrease();

    void setCs10(bool newVal);

private:

    int _tcnt1; //counter

    WaveFormGenerator _waveFormGenerator;
    ClockSelect _clockSelect;
};

#endif // ATMEGATIMER_H
