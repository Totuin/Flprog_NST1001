#include "flprogNST1001.h"

FLProgNST1001::FLProgNST1001(int16_t pin, bool extPower)
{
    _pin = pin;
    _extPower = extPower;
}

void FLProgNST1001::pool()
{
    if (!_isInit)
    {
        init();
        return;
    }
    if (!_isReadMode)
    {
        return;
    }
    if (!_oldIsReadMode)
    {
        _oldIsReadMode = true;
        _startReadTime = millis();
        return;
    }
    if (_hasNewImpulse)
    {
        _startReadTime = millis();
        _hasNewImpulse = false;
        return;
    }
    if (flprog::isTimer(_startReadTime, 5))
    {
        _oldIsReadMode = false;
        _isReadMode = false;
        _raw = _count;
        _count = 0;
        _temperature = filterNST.filtered(getTemperature());
    }
}

void FLProgNST1001::init()
{
    if (_extPower)
    {
        pinMode(_pin, INPUT);
    }
    else
    {
        pinMode(_pin, INPUT_PULLUP);
    }
    _isInit = true;
}

void FLProgNST1001::interrupt(void)
{
    _count++;
    _isReadMode = true;
    _hasNewImpulse = true;
}

float FLProgNST1001::getTemperature()
{
    float temp = (_raw * 0.0625) - 50.0625;
    if ((_raw >= 1281) && (_raw <= 2401))
    {
        return temp;
    }
    if (_raw < 1281)
    {
        temp = temp + ((temp - 30) * 0.005);
        return temp;
    }
    temp = temp + ((100 - temp) * 0.012);
    return temp;
}
