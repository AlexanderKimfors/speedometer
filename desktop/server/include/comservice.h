#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "settings.h"
#include <cstdint>

class COMService
{
private:
    Settings::Signal &signal{Settings::Signal::getInstance()};
    uint8_t buffer[Settings::Signal::BUFFER_SIZE]{};

    void insert(uint8_t data, const char *key);

protected:
    void set_speed(uint8_t data);
    void set_temperature(uint8_t data);
    void set_battery(uint8_t data);
    void set_left_light(uint8_t data);
    void set_right_light(uint8_t data);

    /// @brief Send the buffer via any given communication protocol in derived classes.
    virtual void run() = 0;

    /// @brief Return true if there is a connection.
    virtual bool get_connection_state() = 0;
};

#endif