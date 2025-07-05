#include "comservice.h"

void COMService::insert(uint8_t data, const char *key)
{
    uint8_t byte_index{static_cast<uint8_t>(signal[key].bit_offset / CHAR_BIT)};
    uint8_t bit_index{static_cast<uint8_t>(signal[key].bit_offset % CHAR_BIT)};

    for (int i = 0; i < signal[key].bit_size; i++)
    {
        uint8_t bit = (data >> i) & 1;

        if (bit == 0) // clear the bit at the position
        {
            buffer[byte_index] &= ~(1 << bit_index);
        }
        else // set the bit at the position
        {
            buffer[byte_index] |= bit << bit_index;
        }

        bit_index++;

        if (bit_index == CHAR_BIT)
        {
            bit_index = 0;
            byte_index++;
        }
    }
}

void COMService::set_speed(uint8_t data)
{
    insert(data, "speed");
}
void COMService::set_temperature(uint8_t data)
{
    insert(data, "temperature");
}
void COMService::set_battery(uint8_t data)
{
    insert(data, "battery");
}
void COMService::set_left_light(uint8_t data)
{
    insert(data, "left_light");
}
void COMService::set_right_light(uint8_t data)
{
    insert(data, "right_light");
}
