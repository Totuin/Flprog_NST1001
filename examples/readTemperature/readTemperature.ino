#include "flprogNST1001.h"
#include "RT_HW_CONSOLE.h"
FLProgNST1001 sensor(20);
RT_HW_PIN_DIR_ID idPinLed;
uint8_t pinLed = 25;

void setup()
{
    attachInterrupt(sensor.interruptNumber(), nstInterrup, FALLING);
}

void loop()
{
    RT_HW_Base.sheduler();
    RT_HW_console.begin();
    sensor.pool();
    if (RT_HW_Base.shed.eventSec)
    {
        RT_HW_console.outTest(String(F("T:")), sensor.getTemperature(), ';');
        RT_HW_console.outTest(String(F(" FT:")), sensor.getFilteredTemperature(), ';');
        RT_HW_console.outTest(String(F(" RAW:")), sensor.getRAW());
    }
    RT_HW_Base.pinDigitalWrite(idPinLed, pinLed, RT_HW_Base.shed.blink500); //--Вывод  на пин;
}

void nstInterrup()
{
    sensor.interrupt();
}