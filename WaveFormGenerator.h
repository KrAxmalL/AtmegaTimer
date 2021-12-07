#ifndef WAVEFORMGENERATOR_H
#define WAVEFORMGENERATOR_H

#include <string>

#include "BitUtils.h"

class WaveFormGenerator
{
public:

    //only 4 bits needed
    enum class Mode: int
    {
        Normal = 0x0,
        PWM8Ph = 0x1,
        PWM9Ph = 0x2,
        PWM10Ph = 0x3,
        CTCOcr = 0x4,
        FastPWM8 = 0x5,
        FastPWM9 = 0x6,
        FastPWM10 = 0x7,
        PWMPhFrIcr = 0x8,
        PWMPhFrOcr = 0x9,
        PWMPhIcr = 0xA,
        PWMPhOcr = 0xB,
        CTCIcr = 0xC,
        Reserved = 0xD,
        FastPWMIcr = 0xE,
        FastPWMOcr = 0xF,
    };

    //only for ocr1a changing
    //but using also for icr1a - for simplicity
    enum class RegUpdateMode: int
    {
        Immediate,
        Top,
        Bottom,
        Reserved
    };

    enum class Tov1UpdateMode: int
    {
        Max,
        Top,
        Bottom,
        Reserved
    };

    static constexpr int Max = 0xFFFF;
    static constexpr int Bottom = 0x0000;
    static constexpr int Top_8bit = 0x00FF;
    static constexpr int Top_9bit = 0x01FF;
    static constexpr int Top_10bit = 0x03FF;

    WaveFormGenerator(int ocr1a, int icr1, Mode mode);
    WaveFormGenerator(int ocr1a, int icr1, bool wgm13, bool wgm12, bool wgm11, bool wgm10);

    ~WaveFormGenerator();

    inline bool wgm10() const {return _wgm10;}
    inline bool wgm11() const {return _wgm11;}
    inline bool wgm12() const {return _wgm12;}
    inline bool wgm13() const {return _wgm13;}

    inline int ocr1a() const {return _ocr1a;}
    inline int icr1() const {return _icr1;}

    inline Mode mode() const {return _mode;}

    RegUpdateMode regUpdateMode() const;
    Tov1UpdateMode tov1UpdateMode() const;

    /*bool isNormalMode();
    bool isCtcMode();
    bool isFastPwmMode();
    bool isPhaseCorrectMode();
    bool isPhaseAndFrequencyCorrectMode();*/

    void setWgm10(bool newVal);
    void setWgm11(bool newVal);
    void setWgm12(bool newVal);
    void setWgm13(bool newVal);

    void setIcr1(int newVal);
    void setOcr1a(int newVal);

    void setMode(Mode mode);

    std::string description() const;

    int top() const;

private:

    static constexpr Mode modes[] = { Mode::Normal, Mode::PWM8Ph, Mode::PWM9Ph, Mode::PWM10Ph,
                                      Mode::CTCOcr, Mode::FastPWM8, Mode::FastPWM9, Mode::FastPWM10,
                                      Mode::PWMPhFrIcr,  Mode::PWMPhFrOcr, Mode::PWMPhIcr,  Mode::PWMPhOcr,
                                      Mode::CTCIcr, Mode::Reserved, Mode::FastPWMIcr, Mode::FastPWMOcr };

    bool _wgm10;
    bool _wgm11;
    bool _wgm12;
    bool _wgm13;

    int _ocr1a;
    int _icr1;

    int _reg;

    Mode _mode;

};

#endif // WAVEFORMGENERATOR_H
