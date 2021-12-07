#ifndef ATMEGATIMER_H
#define ATMEGATIMER_H

#include <QWidget>

#include "WaveFormGenerator.h"
#include "ClockSelect.h"

class AtmegaTimer : public QWidget
{
    Q_OBJECT

public:

    AtmegaTimer(long t1, long inputFrequency, int icr1, int ocr1a,
                bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                bool cs12, bool cs11, bool cs10, bool com1a1, bool com1a0);

    virtual ~AtmegaTimer();

    inline int tcnt1() const {return _tcnt1;}
    inline bool tov1() const {return _tov1;}
    inline bool oc1a() const {return _oc1a;}

    inline bool com1a1() const {return _com1a1;}
    inline bool com1a0() const {return _com1a0;}

    int top() const;

    long t1();
    long clk() const;
    long actualClk() const;

    bool cs10() const;
    bool cs11() const;
    bool cs12() const;

    int icr1();

    int ocr1a();

    bool wgm10() const;
    bool wgm11() const;
    bool wgm12() const;
    bool wgm13() const;

    void setCs10(bool newVal);
    void setCs11(bool newVal);
    void setCs12(bool newVal);

    void setWgm10(bool newVal);
    void setWgm11(bool newVal);
    void setWgm12(bool newVal);
    void setWgm13(bool newVal);

    void setCom1a0(bool newVal);
    void setCom1a1(bool newVal);

    void setOc1a(bool newVal);
    void setTov1(bool newVal);

    void setClk(long frequency);
    void setT1(long frequency);

    void setOcr1a(int val);
    void setIcr1(int val);

    void performClock();

    void increase();
    void decrease();

    void normalStep();
    void ctcStep();
    void fastPwmStep();
    void phaseCorrectStep();
    void phaseAndFrequencyCorrectStep();

signals:

    void tov1Changed();
    void oc1aChanged();
    void actualClkChanged();
    void topChanged();

private:

    int _tcnt1; //counter

    bool _tov1;
    bool _oc1a;

    bool _com1a0;
    bool _com1a1;

    WaveFormGenerator _waveFormGenerator;
    ClockSelect _clockSelect;
};

#endif // ATMEGATIMER_H
