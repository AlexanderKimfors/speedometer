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

    uint8_t temp_buffer[BUFFLEN]{0};

    while (true)
    {
        if (serial.open(QIODevice::WriteOnly))
        {
            printf("Serial port opened successfully.\n");
            while (1)
            {
                // Write to temp buffer
                {
                    std::scoped_lock lock(buffer_mtx);
                    memcpy(temp_buffer, buffer, BUFFLEN);
                }

                const qint64 data_written = serial.write(reinterpret_cast<char *>(temp_buffer), BUFFLEN);

                if (data_written < 0)
                {
                    qDebug() << "Error writing to serial port:" << serial.errorString();
                    serial.close(); // force reopen next iteration
                    QThread::msleep(200);
                    break;
                }

                if (!serial.waitForBytesWritten(200))
                {
                    qWarning() << "waitForBytesWritten timeout:" << serial.errorString();
                    // optional: serial.clearError();
                }

                QThread::msleep(settings::DRAW_INTERVAL); // Sleep for a defined interval synced with the draw interval
            }
        }
        else
        {
            qDebug() << "Could not open the port:" << serial.errorString();
            QThread::msleep(1000); // Wait before retrying to open the port
        }

        serial.close();        // Close the port before the next iteration
        QThread::msleep(1000); // Wait before trying to open the port again
    }
}