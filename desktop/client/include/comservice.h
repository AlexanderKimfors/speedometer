#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <mutex>
#include <stdint.h>

class COMService
{
private:
    Settings::Signal &signal{Settings::Signal::getInstance()};

    uint8_t extract(const char *key);

protected:
    uint8_t buffer[BUFFLEN]{};
    mutable std::mutex buffer_mutex;

public:
    uint8_t get_speed(void);
    int8_t get_temperature(void);
    uint8_t get_battery(void);
    uint8_t get_left_light(void);
    uint8_t get_right_light(void);

    /// @brief Receive the buffer via any given communication protocol in derived classes.
    virtual void run() = 0;

    /// @brief Return true if there is a connection.
    virtual bool get_connection_state() = 0;
};

#endif