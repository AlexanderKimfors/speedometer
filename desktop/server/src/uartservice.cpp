#include "setting.h"
#include <QSerialPort>
#include "uartservice.h"
#include <QDebug>

void UARTService::run()
{
    QSerialPort serial;
    uint8_t temp[sizeof(buffer)]{0};

    serial.setPortName(UART_SPORT);
    serial.setBaudRate(BAUDRATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    while (!end)
    {
        if (serial.open(QIODevice::WriteOnly))
        {
            qDebug() << "Serial port is opened";

            (void)serial.clear();

            while (!end && serial.isWritable())
            {
                qDebug() << "Serial port is writable";

                {
                    std::scoped_lock<std::mutex> lock{buffer_mtx};
                    std::memcpy(temp, buffer, sizeof(buffer));
                }

                if (sizeof(temp) == serial.write(reinterpret_cast<const char *>(temp), sizeof(temp)))
                {
                    if (serial.flush())
                    {
                        status = true;
                        QThread::msleep(settings::DRAW_INTERVAL);
                    }
                    else
                    {
                        status = false;
                        break;
                    }
                }
                else
                {
                    status = false;
                    break;
                }
            }
        }
        else
        {
            status = false;
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}
