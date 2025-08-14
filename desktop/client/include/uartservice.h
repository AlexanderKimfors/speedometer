#ifndef UARTCOM_H
#define UARTCOM_H

#include <QThread>
#include <QDebug>
#include <QSerialPort>
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

    void run() override
    {
        QSerialPort serial;

        serial.setPortName(settings::SerialPort::CLIENT_PORT);
        serial.setBaudRate(BAUDRATE);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);

        serial.open(QIODevice::ReadOnly);

        qDebug() << "Serial port is opened";

        uint8_t buffer{0};

        while (true)
        {
            if (serial.waitForReadyRead(1000))
            { // väntar max 1 sekund
                qint64 bytesRead = serial.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
                if (bytesRead > 0)
                {
                    qDebug() << "Data mottagen";
                    for (int i = 0; i < bytesRead; i++)
                    {
                        qDebug() << buffer;
                    }
                }
                else
                {
                    qDebug() << "Tom data mottagen";
                }
            }
            else
            {
                qDebug() << "Ingen data mottagen inom 1 sekund";
            }
        }
    }
};

#endif