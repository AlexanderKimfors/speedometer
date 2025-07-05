#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <cstdint>
#include "settings.h"

class COMService
{
private:
    Settings::Signal &signal{Settings::Signal::getInstance()};
    uint8_t buffer[Settings::Signal::BUFFER_SIZE]{};

    uint8_t extract(const char *key);

protected:
    uint8_t get_speed(void);
    int8_t get_temperature(void);
    uint8_t get_battery(void);
    uint8_t get_left_light(void);
    uint8_t get_right_light(void);
};

#endif

/*




v.Receiving the buffer over the chosen communication protocol.
vi.Getting status of the communication.
*/
