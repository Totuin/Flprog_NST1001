#include "flprogNST1001.h"

#if defined(__AVR__)
#include <util/atomic.h>
#endif

FLProgNST1001::FLProgNST1001(int16_t pin, bool extPower)
    : _pin(pin)
{
    // При внешнем питании/подтяжке датчика вход не подтягиваем программно.
    pinMode(_pin, extPower ? INPUT : INPUT_PULLUP);
}

void FLProgNST1001::interrupt()
{
    // Читаем время только один раз: датчик выдаёт импульсы с периодом ~8 мкс.
    // Вычитание uint32_t корректно и при переполнении 32-битного millis().
    const uint32_t now = millis();
    if (static_cast<uint32_t>(now - _lastInterruptTime) >= 10UL)
    {
        // Между импульсами прошла пауза: предыдущая серия закончена.
        // Первое событие новой серии публикует результат предыдущей.
        _raw = _count;
        _count = 0;
    }
    ++_count; // Текущий импульс принадлежит новой либо продолжающейся серии.
    _lastInterruptTime = now;
}

uint16_t FLProgNST1001::getRAW()
{
#if defined(__AVR__)
    // На 8-битном AVR чтение uint16_t состоит из нескольких операций.
    // ATOMIC_RESTORESTATE восстанавливает прежний флаг прерываний, а не
    // включает их безусловно (в отличие от noInterrupts()/interrupts()).
    uint16_t value;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        value = _raw;
    }
    return value;
#else
    // На поддерживаемых 32-битных ядрах выровненное чтение uint16_t атомарно;
    // volatile не позволяет кэшировать обновляемое в ISR значение.
    return _raw;
#endif
}

float FLProgNST1001::getTemperature()
{
    // Получаем один согласованный снимок: в момент расчёта ISR может
    // опубликовать очередной кадр.
    const uint16_t raw = getRAW();

    // Уравнения (4-1)/(4-2) из datasheet NOVOSENSE NST1001 Rev. 1.12.
    // Диапазон датчика 1..3201 импульсов; коэффициенты НЕ менялись.
    float temp = (raw * 0.0625f) - 50.0625f;
    if ((raw >= 1281U) && (raw <= 2401U))
    {
        return temp;
    }
    if (raw < 1281U)
    {
        temp += ((temp - 30.0f) * 0.005f);
        return temp;
    }
    temp += ((100.0f - temp) * 0.012f);
    return temp;
}
