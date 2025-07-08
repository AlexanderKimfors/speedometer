#include "comservice.h"

uint8_t COMService::extract(const char *key)
{
    uint8_t data{0};

    int byte_index = signal[key].bit_offset / CHAR_BIT;
    int bit_index = signal[key].bit_offset % CHAR_BIT;

    for (int i = 0; i < signal[key].bit_size; i++)
    {
        uint8_t bit = (buffer[byte_index] >> bit_index) & 0b00000001;
        data |= (bit << i);

        bit_index++;

        if (bit_index == CHAR_BIT)
        {
            bit_index = 0;
            byte_index++;
        }
    }

    return data;
}

uint8_t COMService::get_speed(void)
{
    return extract("speed");
}
int8_t COMService::get_temperature(void)
{
    uint8_t data = extract("temperature");
    uint8_t MSB_index = signal["temperature"].bit_size - 1;

    // check if the data is negativ
    if (data & (1 << MSB_index))
    {
        data |= ~((1 << (MSB_index + 1)) - 1); // set extended bits to 1
    }

    return static_cast<int8_t>(data);
}
uint8_t COMService::get_battery(void)
{
    return extract("battery");
}
uint8_t COMService::get_left_light(void)
{
    return extract("left_light");
}
uint8_t COMService::get_right_light(void)
{
    return extract("right_light");
}
