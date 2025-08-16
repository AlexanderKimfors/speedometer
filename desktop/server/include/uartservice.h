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
            quit();
            wait();
        }

        void run() override;
    
        
    private:
};

#endif
