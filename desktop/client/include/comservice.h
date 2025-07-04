#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <cstdint>
#include "settings.h"

class Comservice
{
private:
    Settings::Signal &signal{Settings::Signal::getInstance()};
    uint8_t buffer[Settings::Signal::BUFFER_SIZE]{};
};

#endif

/*
i.Extractingspeedfromthebuffer.
ii.Extractingtemperaturefromthebuffer.
iii.Extractingbatterylevelfromthebuffer.
iv.Extractingthelightsignalstatesfromthebuffer.
v.Receivingthebufferoverthechosencommunicationprotocol.
vi.Gettingstatusofthecommunication.
*/
