#include <cassert>
#include <cmath>
#include <cstdint>
#include "flprogNST1001.h"

uint32_t fakeMillis = 0;
int fakeLastPin = -1;
int fakeLastPinMode = -1;
int atomicBlockEnter = 0;
int atomicBlockExit = 0;
SerialStub Serial;
uint32_t millis() { return fakeMillis; }
void pinMode(int pin, int mode) { fakeLastPin = pin; fakeLastPinMode = mode; }
uint8_t digitalPinToInterrupt(int pin) { return static_cast<uint8_t>(pin); }
void attachInterrupt(uint8_t, void (*)(), int) {}

static void frame(FLProgNST1001& sensor, uint16_t count) {
  fakeMillis += 20U; // 20 мс между сериями, больше порога 10 мс.
  for (uint16_t i=0; i<count; ++i) sensor.interrupt();
}
static void assertNear(float actual, float expected) {
  assert(std::fabs(actual-expected)<0.001f);
}

int main() {
  fakeMillis = 0;
  FLProgNST1001 sensor(2);
  assert(fakeLastPin==2 && fakeLastPinMode==INPUT_PULLUP);
  assert(sensor.interruptNumber()==2);
  assert(sensor.getRAW()==0);
  FLProgNST1001 withExternalPullUp(3, true);
  assert(fakeLastPin==3 && fakeLastPinMode==INPUT);

  frame(sensor, 1281);
  // Текущее измерение не опубликовано до начала следующего.
  assert(sensor.getRAW()==0);
  frame(sensor, 2401);
  assert(sensor.getRAW()==1281);
  assertNear(sensor.getTemperature(), 30.0f);
  frame(sensor, 1);
  assert(sensor.getRAW()==2401);
  assertNear(sensor.getTemperature(), 100.0f);
  frame(sensor, 801);
  assert(sensor.getRAW()==1);
  assertNear(sensor.getTemperature(), -50.4f);
  frame(sensor, 3201);
  assert(sensor.getRAW()==801);
  assertNear(sensor.getTemperature(), -0.15f);
  frame(sensor, 300);
  assert(sensor.getRAW()==3201);
  assertNear(sensor.getTemperature(), 149.4f);

  // rollover unsigned millis(): интервал от FFFFFFF8 до 0000000F = 23 мс.
  fakeMillis=UINT32_MAX - 7U;
  FLProgNST1001 rollover(4);
  rollover.interrupt();
  rollover.interrupt();
  fakeMillis=15;
  rollover.interrupt();
  assert(rollover.getRAW()==2);

#if defined(__AVR__)
  assert(atomicBlockEnter>0 && atomicBlockEnter==atomicBlockExit);
#else
  assert(atomicBlockEnter==0);
#endif
  return 0;
}
