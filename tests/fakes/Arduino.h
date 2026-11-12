#pragma once
#include <stdint.h>
#define INPUT 0
#define INPUT_PULLUP 2
#define FALLING 3
#define F(x) x
extern uint32_t fakeMillis;
extern int fakeLastPin;
extern int fakeLastPinMode;
uint32_t millis();
void pinMode(int, int);
uint8_t digitalPinToInterrupt(int);
void attachInterrupt(uint8_t, void (*)(), int);
class SerialStub {
public:
 void begin(unsigned long) {}
 void print(const char*) {}
 void print(unsigned int) {}
 void println(const char*) {}
 void println(float, int) {}
};
extern SerialStub Serial;
