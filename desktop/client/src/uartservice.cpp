#include <QDebug>
#include <QSerialPort>
#include "uartservice.h"

void UARTService::run()
{
    QSerialPort serial;

    serial.setPortName(UART_CPORT);
    serial.setBaudRate(BAUDRATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while (!end)
    {
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

        while (!end && status && serial.isOpen())
        {
            if (serial.waitForReadyRead(settings::DRAW_INTERVAL * 3))
            { // väntar max 1 sekund
                qint64 bytesRead = serial.read(reinterpret_cast<char *>(temp_buffer), BUFFLEN);

                if (BUFFLEN == bytesRead)
                {
                    std::scoped_lock lock(buffer_mutex);
                    memcpy(buffer, temp_buffer, BUFFLEN);
                }
                else
                {
                    qDebug() << "Could not read the entire message.";
                    status = false;
                    serial.close();
                }
            }
            else
            {
                qDebug() << "No data received in the time limit.";
                status = false;
                serial.close();
            }
        }
    }
}