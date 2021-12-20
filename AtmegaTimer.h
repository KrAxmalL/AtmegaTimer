#ifndef ATMEGATIMER_H
#define ATMEGATIMER_H

#include <QWidget>

#include "WaveFormGenerator.h"
#include "ClockSelect.h"

class AtmegaTimer : public QWidget
{
    Q_OBJECT

public:

    enum class OutputCompareMode: int
    {
        Disconnected = 0,
        Toggle = 1,
        Normal = 2,
        Inverted = 3,
    };

    AtmegaTimer(long t1, long inputFrequency, int icr1, int ocr1a, int ocr1b,
                bool wgm13, bool wgm12, bool wgm11, bool wgm10,
                bool cs12, bool cs11, bool cs10, bool com1a1, bool com1a0, bool com1b1, bool com1b0);

    virtual ~AtmegaTimer();

    inline int tcnt1() const {return _tcnt1;}
    inline bool tov1() const {return _tov1;}
    inline bool oc1a() const {return _oc1a;}
    inline bool oc1b() const {return _oc1b;}

    inline bool com1a1() const {return _com1a1;}
    inline bool com1a0() const {return _com1a0;}
    inline bool com1b1() const {return _com1b1;}
    inline bool com1b0() const {return _com1b0;}

    int top() const;

    long t1();
    long clk() const;
    long actualClk() const;

    bool cs10() const;
    bool cs11() const;
    bool cs12() const;

    int icr1();

    int ocr1a();
    int ocr1b();
    int ocr1aBuffer();
    int ocr1bBuffer();

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
    void setCom1b0(bool newVal);
    void setCom1b1(bool newVal);

    void setOc1a(bool newVal);
    void setOc1b(bool newVal);
    void setTov1(bool newVal);

    void setClk(long frequency);
    void setT1(long frequency);

    void setOcr1a(int val);
    void setOcr1b(int val);
    void loadOcr1aFromBuffer();
    void loadOcr1bFromBuffer();
    void setIcr1(int val);

    int comparableValueA();
    int comparableValueB();

    WaveFormGenerator::Mode timerMode();
    ClockSelect::State timerState();
    AtmegaTimer::OutputCompareMode outputModeA();
    AtmegaTimer::OutputCompareMode outputModeB();

signals:

    void tov1Changed();
    void oc1aChanged();
    void oc1bChanged();
    void actualClkChanged();
    void topChanged();

private:

    int _tcnt1;

    bool _tov1;
    bool _oc1a;
    bool _oc1b;

    bool _com1a0;
    bool _com1a1;

    bool _com1b0;
    bool _com1b1;

    OutputCompareMode _outputModeA;
    OutputCompareMode _outputModeB;

    WaveFormGenerator _waveFormGenerator;
    ClockSelect _clockSelect;
};

#endif // ATMEGATIMER_H
