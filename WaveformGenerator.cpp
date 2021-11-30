#include "WaveFormGenerator.h"

WaveFormGenerator::WaveFormGenerator(int ocr1a, int icr1a, Mode mode): _ocr1a(ocr1a), _icr1a(icr1a),
                                                                       _mode(mode)
{
    _reg = static_cast<int>(_mode);
    _wgm10 = 0x0001 & _reg;
    _wgm11 = 0x0002 & _reg;
    _wgm12 = 0x0004 & _reg;
    _wgm13 = 0x0008 & _reg;
}

WaveFormGenerator::WaveFormGenerator(int ocr1a, int icr1a, bool wgm13,
                                     bool wgm12, bool wgm11, bool wgm10): _ocr1a(ocr1a), _icr1a(icr1a),
                                                                          _wgm13(wgm13), _wgm12(wgm12),
                                                                          _wgm11(wgm11), _wgm10(wgm10)
{
    _reg |= _wgm10;
    _reg |= (_wgm11 << 1);
    _reg |= (_wgm12 << 2);
    _reg |= (_wgm13 << 3);

    _mode = static_cast<Mode>(_reg);
}

WaveFormGenerator::~WaveFormGenerator() {}

void WaveFormGenerator::setWgm10(bool newVal)
{
    _wgm10 = newVal;
    _reg |= _wgm10;
     _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm11(bool newVal)
{
    _wgm11 = newVal;
    _reg |= (_wgm11 << 1);
     _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm12(bool newVal)
{
    _wgm12 = newVal;
    _reg |= (_wgm12 << 2);
     _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm13(bool newVal)
{
    _wgm13 = newVal;
    _reg |= (_wgm13 << 3);
     _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setIcr1a(int newVal)
{
    _icr1a = newVal;
}

void WaveFormGenerator::setOcr1a(int newVal)
{
    _ocr1a = newVal;
}

void WaveFormGenerator::setMode(Mode mode)
{
    _mode = mode;
    _reg = static_cast<int>(_mode);
    _wgm10 = 0x0001 & _reg;
    _wgm11 = 0x0002 & _reg;
    _wgm12 = 0x0004 & _reg;
    _wgm13 = 0x0008 & _reg;
}

std::string WaveFormGenerator::description() const
{
    return "";
}

int WaveFormGenerator::top() const
{
    switch(_mode)
    {
        case Mode::Normal: return Max;

        case Mode::PWM8Ph: case Mode::FastPWM8: return Top_8bit;

        case Mode::PWM9Ph: case Mode::FastPWM9: return Top_9bit;

        case Mode::PWM10Ph: case Mode::FastPWM10: return Top_10bit;

        case Mode::CTCIcr: case Mode::FastPWMIcr:
        case Mode::PWMPhFrIcr: case Mode::PWMPhIcr: return _icr1a;

        case Mode::CTCOcr: case Mode::FastPWMOcr:
        case Mode::PWMPhFrOcr: case Mode::PWMPhOcr: return _ocr1a;

        case Mode::Reserved: return 0; //not used
    }
}
