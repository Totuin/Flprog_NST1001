#include <flprogNST1001.h>

// Выберите вход, поддерживающий внешние прерывания на вашей плате.
// На Arduino Uno это, например, D2; проверьте разводку и уровни DQ.
const uint8_t kSensorPin = 2;
FLProgNST1001 sensor(kSensorPin); // По умолчанию INPUT_PULLUP.

// Обработчик должен быть глобальной функцией без параметров.
void nstInterrupt()
{
    sensor.interrupt();
}

void setup()
{
    Serial.begin(115200);
    // Обратите внимание: на неподдерживаемом GPIO digitalPinToInterrupt()
    // может вернуть NOT_AN_INTERRUPT; выберите правильный вход платы.
    attachInterrupt(sensor.interruptNumber(), nstInterrupt, FALLING);
}

void loop()
{
    static uint32_t lastPrint = 0;
    const uint32_t now = millis();
    if (static_cast<uint32_t>(now - lastPrint) < 500UL)
    {
        return;
    }
    lastPrint = now;

    const uint16_t raw = sensor.getRAW();
    if (raw == 0)
    {
        Serial.println(F("NST1001: ожидаем завершения первой серии"));
        return;
    }
    if (raw > 3201U)
    {
        // Значение вне диапазона датчика: возможны помехи/ошибки подсчёта.
        Serial.println(F("NST1001: некорректное число импульсов"));
        return;
    }

    Serial.print(F("NST1001 RAW: "));
    Serial.print(raw);
    Serial.print(F("; T: "));
    Serial.println(sensor.getTemperature(), 2);
}
