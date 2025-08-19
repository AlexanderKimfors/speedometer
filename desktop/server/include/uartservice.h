#ifndef UARTCOM_H
#define UARTCOM_H

#include "comservice.h"
#include <QThread>
#include <QSerialPort>

class UARTService : public ComService, public QThread
{
public:
    UARTService()
    {
        start();
    }

    ~UARTService()
    {
        end = true;
        quit();
        wait();
    }

private:
    std::atomic<bool> end{false};
    void run() override;

private:
};

#endif
