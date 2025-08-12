#include "comservice.h"

void ComService::setSpeed(uint32_t speed)
{
    insert(signal["speed"].bit_offset, signal["speed"].bit_size, speed);
}

void ComService::setTemperature(int32_t temperature)
{
    insert(signal["temperature"].bit_offset, signal["temperature"].bit_size, temperature);
}

void ComService::setBatteryLevel(uint32_t level)
{
    insert(signal["Battery level"].bit_offset, signal["Battery level"].bit_size, level);
}

void ComService::setRightLight(bool state)
{
    insert(signal["right_light"].bit_offset, signal["right_light"].bit_size, static_cast<uint32_t>(state));
}

void ComService::setLeftLight(bool state)
{
    insert(signal["left_light"].bit_offset, signal["left_light"].bit_size, static_cast<uint32_t>(state));
}