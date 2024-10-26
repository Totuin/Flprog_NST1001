#include "flprogNST1001.h"

/*
FLProgNST1001 *pointerToClassFLProgNST1001; // declare a pointer to testLib class

static void outsideFLProgNST1001InterruptHandler(void) // define global handler
{
    pointerToClassFLProgNST1001->interrupt(); // calls class member handler
}
*/

FLProgNST1001::FLProgNST1001(int16_t pin, bool extPower)
{
    _pin = pin;
    if (extPower)
    {
        pinMode(_pin, INPUT);
    }
    else
    {
        pinMode(_pin, INPUT_PULLUP);
    }
}

void FLProgNST1001::interrupt(void)
{
    if (flprog::isTimer(_lastInterruptTime, 10))
    {
        _raw = _count;
        _count = 0;
    }
    _count++;
    _lastInterruptTime = millis();
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

