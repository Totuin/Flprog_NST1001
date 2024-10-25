#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

class FLProgNST1001
{
public:
    FLProgNST1001(int16_t pin, bool extPower = false);
    void pool();
    uint8_t interruptNumber() { return digitalPinToInterrupt(_pin); }
    float getTemperature();
    uint32_t getRAW() { return _raw; };
    void interrupt();

protected:
    void init();
    bool _isInit = false;
    int16_t _pin = -1;
    bool _extPower;
    volatile uint32_t _count = 0;
    uint32_t _raw = 0;
    volatile bool _isReadMode = false;
    uint32_t _startReadTime;
    bool _oldIsReadMode = false;
    volatile bool _hasNewImpulse = false;
};
