#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "settings.h"
#include <cstdint>

class COMService
{
private:
    Settings::Signal &signal{Settings::Signal::getInstance()};

    void insert(uint8_t data, const char *key);

protected:
    uint8_t buffer[Settings::Signal::BUFFER_SIZE]{};

public:
    void set_speed(uint8_t data);
    void set_temperature(uint8_t data);
    void set_battery(uint8_t data);
    void set_left_light(uint8_t data);
    void set_right_light(uint8_t data);

    /// @brief Send the buffer via any given communication protocol in derived classes.
    virtual void run() = 0;
};

#endif