#include "setting.h"
#include <QSerialPort>
#include "uartservice.h"
#include <QDebug>

void UARTService::run()
{
    QSerialPort serial;

    serial.setPortName(UART_SPORT);
    serial.setBaudRate(BAUDRATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while (!end)
    {
        if (!serial.open(QIODevice::WriteOnly))
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
            {
                std::scoped_lock lock(buffer_mtx);
                memcpy(temp_buffer, buffer, BUFFLEN);
            }

            const qint64 data_written = serial.write(reinterpret_cast<char *>(temp_buffer), BUFFLEN);

            if (BUFFLEN == data_written)
            {
                serial.flush();
            }
            else
            {
                qDebug() << "Error writing to serial port:" << serial.errorString();
                serial.close();
                status = false;
            }

            QThread::msleep(settings::DRAW_INTERVAL);
        }
    }
}
