#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"
#include "RT_HW_BASE.h"

class FLProgNST1001
{
public:
    FLProgNST1001(int16_t pin, bool extPower = false);
    void pool();
    uint8_t interruptNumber() { return digitalPinToInterrupt(_pin); }
    float getTemperature();
    uint16_t getRAW() { return _raw; };
    float getFilteredTemperature() { return _temperature; };
    void interrupt();

protected:
    void init();
    bool _isInit = false;
    int16_t _pin = -1;
    bool _extPower;
    volatile uint16_t _count = 0;
    uint16_t _raw = 0;
    volatile bool _isReadMode = false;
    uint32_t _startReadTime;
    bool _oldIsReadMode = false;
    volatile bool _hasNewImpulse = false;
    float _temperature = 0;
    RT_HW_FILTER_MEDIAN_N<float, 11> filterNST;
};
