#ifndef UARTCOM_H
#define UARTCOM_H

#include <QThread>
#include <QDebug>
#include "comservice.h"
#include "setting.h"

class UARTService : public COMService, public QThread
{
public:
    UARTService()
    {
        start();
    }
    ~UARTService()
    {
        quit();
        wait();
    }

    void run() override;
};

#endif