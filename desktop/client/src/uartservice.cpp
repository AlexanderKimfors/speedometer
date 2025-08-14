#include <QDebug>
#include <QSerialPort>
#include "uartservice.h"

void UARTService::run()
{
    QSerialPort serial;

    serial.setPortName(settings::SerialPort::CLIENT_PORT);
    serial.setBaudRate(BAUDRATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open the port:" << serial.errorString();
        exit(EXIT_FAILURE);
    }
    else
    {
        status = true;
        qDebug() << "Serial port is opened!";
    }

    uint8_t temp_buffer[BUFFLEN]{0};

    while (true)
    {
        /*
        läs från buffern till en temp buffer.
        lås den riktiga buffern och kopiera över temp buffer till den.
        */
        if (serial.waitForReadyRead(1000))
        { // väntar max 1 sekund
            qint64 bytesRead = serial.read(reinterpret_cast<char *>(temp_buffer), BUFFLEN);

            if (bytesRead > 0)
            {
                qDebug() << "Data received:";
                for (int i = 0; i < BUFFLEN; i++)
                {
                    qDebug() << temp_buffer[i];
                }

                qDebug() << "Sending the data to the buffer";
                std::scoped_lock lock(buffer_mutex);
                memcpy(buffer, temp_buffer, BUFFLEN);
            }
        }
        else
        {
            qDebug() << "No data received in the time limit.";
        }
    }
}