#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"
#include "RT_HW_BASE.h"

class FLProgNST1001
{
public:
    FLProgNST1001(int16_t pin, bool extPower = false);
    uint8_t interruptNumber() { return digitalPinToInterrupt(_pin); }
    float getTemperature();
    uint16_t getRAW() { return _raw; };
    void interrupt();

protected:
    int16_t _pin = -1;
    uint8_t filterOrder=11;
    volatile uint16_t _count = 0;
    uint16_t _raw = 0;
    uint32_t _lastInterruptTime = millis();
};
