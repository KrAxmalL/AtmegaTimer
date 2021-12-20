#include "WaveFormGenerator.h"

WaveFormGenerator::WaveFormGenerator(int ocr1a, int ocr1b, int icr1, Mode mode): _ocr1aBuffer(ocr1a), _ocr1bBuffer(ocr1b),
                                                                                 _ocr1a(0), _ocr1b(0), _icr1(icr1),
                                                                                 _mode(mode), _reg(0),
                                                                                 _wgm13(0), _wgm12(0),
                                                                                 _wgm11(0), _wgm10(0)
{
    _reg = static_cast<int>(_mode);
    _wgm10 = 0x0001 & _reg;
    _wgm11 = 0x0002 & _reg;
    _wgm12 = 0x0004 & _reg;
    _wgm13 = 0x0008 & _reg;
}

WaveFormGenerator::WaveFormGenerator(int ocr1a, int ocr1b, int icr1, bool wgm13,
                                     bool wgm12, bool wgm11, bool wgm10): _ocr1aBuffer(ocr1a),  _ocr1bBuffer(ocr1b),
                                                                          _ocr1a(0), _ocr1b(0), _icr1(icr1),
                                                                          _wgm13(wgm13), _wgm12(wgm12),
                                                                          _wgm11(wgm11), _wgm10(wgm10),
                                                                          _reg(0), _mode(WaveFormGenerator::Mode::Normal)
{
    _reg = setBitNToX(_reg, 0, _wgm10);
    _reg = setBitNToX(_reg, 1, _wgm11);
    _reg = setBitNToX(_reg, 2, _wgm12);
    _reg = setBitNToX(_reg, 3, _wgm13);

    _mode = static_cast<Mode>(_reg);
}

WaveFormGenerator::~WaveFormGenerator() {}

void WaveFormGenerator::setWgm10(bool newVal)
{
    _wgm10 = newVal;
    _reg = setBitNToX(_reg, 0, _wgm10);
    _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm11(bool newVal)
{
    _wgm11 = newVal;
    _reg = setBitNToX(_reg, 1, _wgm11);
    _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm12(bool newVal)
{
    _wgm12 = newVal;
    _reg = setBitNToX(_reg, 2, _wgm12);
    _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setWgm13(bool newVal)
{
    _wgm13 = newVal;
    _reg = setBitNToX(_reg, 3, _wgm13);
    _mode = static_cast<Mode>(_reg);
}

void WaveFormGenerator::setIcr1(int newVal)
{
    _icr1 = newVal;
}

void WaveFormGenerator::setOcr1a(int newVal)
{
    _ocr1a = newVal;
}

void WaveFormGenerator::setOcr1b(int newVal)
{
    _ocr1b = newVal;
}

void WaveFormGenerator::setOcr1aBuffer(int newVal)
{
    _ocr1aBuffer = newVal;
}

void WaveFormGenerator::setOcr1bBuffer(int newVal)
{
    _ocr1bBuffer = newVal;
}

void WaveFormGenerator::loadOcr1aFromBuffer()
{
    _ocr1a = _ocr1aBuffer;
}

void WaveFormGenerator::loadOcr1bFromBuffer()
{
    _ocr1b = _ocr1bBuffer;
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
        case Mode::PWMPhFrIcr: case Mode::PWMPhIcr: return _icr1;

        case Mode::CTCOcr: case Mode::FastPWMOcr:
        case Mode::PWMPhFrOcr: case Mode::PWMPhOcr: return _ocr1a;

        case Mode::Reserved: return 0; //not used

        default: throw std::exception();
    }
}

int  WaveFormGenerator::comparableValueA()
{
    switch(_mode)
    {
        case Mode::Normal: return Max;

        case Mode::CTCIcr: return _icr1;

        case Mode::PWM8Ph: case Mode::FastPWM8: case Mode::PWM9Ph:
        case Mode::FastPWM9: case Mode::PWM10Ph: case Mode::FastPWM10:
        case Mode::FastPWMIcr: case Mode::PWMPhFrIcr: case Mode::PWMPhIcr:
        case Mode::CTCOcr: case Mode::FastPWMOcr: case Mode::PWMPhFrOcr:
        case Mode::PWMPhOcr: return _ocr1a;

        case Mode::Reserved: return 0; //not used

        default: throw std::exception();
    }
}

int  WaveFormGenerator::comparableValueB()
{
    switch(_mode)
    {
        case Mode::Normal: return Max;

        case Mode::CTCIcr: return _icr1;

        case Mode::PWM8Ph: case Mode::FastPWM8: case Mode::PWM9Ph:
        case Mode::FastPWM9: case Mode::PWM10Ph: case Mode::FastPWM10:
        case Mode::FastPWMIcr: case Mode::PWMPhFrIcr: case Mode::PWMPhIcr:
        case Mode::CTCOcr: case Mode::FastPWMOcr: case Mode::PWMPhFrOcr:
        case Mode::PWMPhOcr: return _ocr1b;

        case Mode::Reserved: return 0; //not used

        default: throw std::exception();
    }
}

WaveFormGenerator::RegUpdateMode WaveFormGenerator::regUpdateMode() const
{
    switch(_mode)
    {
        case Mode::Normal: case Mode::CTCOcr: case Mode::CTCIcr: return WaveFormGenerator::RegUpdateMode::Immediate;

        case Mode::PWM8Ph: case Mode::PWM9Ph: case Mode::PWM10Ph:
        case Mode::PWMPhOcr: case Mode::PWMPhIcr: return WaveFormGenerator::RegUpdateMode::Top;

        case Mode::FastPWM8: case Mode::FastPWM9: case Mode::FastPWM10: case Mode::FastPWMOcr:
        case Mode::FastPWMIcr: case Mode::PWMPhFrOcr: case Mode::PWMPhFrIcr: return WaveFormGenerator::RegUpdateMode::Bottom;

        case Mode::Reserved: return WaveFormGenerator::RegUpdateMode::Reserved; //not used
    }
}

WaveFormGenerator::Tov1UpdateMode WaveFormGenerator::tov1UpdateMode() const
{
    switch(_mode)
    {
        case Mode::Normal: case Mode::CTCOcr: case Mode::CTCIcr: return WaveFormGenerator::Tov1UpdateMode::Max;

        case Mode::FastPWM8: case Mode::FastPWM9: case Mode::FastPWM10:
        case Mode::FastPWMOcr: case Mode::FastPWMIcr: return WaveFormGenerator::Tov1UpdateMode::Top;

        case Mode::PWM8Ph: case Mode::PWM9Ph: case Mode::PWM10Ph: case Mode::PWMPhFrOcr:
        case Mode::PWMPhFrIcr: case Mode::PWMPhOcr: case Mode::PWMPhIcr: return WaveFormGenerator::Tov1UpdateMode::Bottom;

        case Mode::Reserved: return WaveFormGenerator::Tov1UpdateMode::Reserved; //not used
    }
}
