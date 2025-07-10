#include "serialportservice.h"
#include <thread>
#include <QDebug>
#include <unistd.h>

SerialPortService::SerialPortService()
{
    serial.setBaudRate(Settings::SerialPort::BAUD_RATE);
    serial.setPortName(Settings::SerialPort::CLIENT_PORT);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);

    worker_thread = std::thread(&SerialPortService::run, this);
}

void SerialPortService::run()
{
    while (!end)
    {
        if (connected && serial.isWritable())
        {
            serial.clear();
            uint8_t temp_buffer[sizeof(buffer)]{0};
            if (serial.waitForReadyRead(Settings::SerialPort::WAIT_FOR_DATA_MS)) // read is not blocking
            {
                if (sizeof(buffer) == serial.read(reinterpret_cast<char *>(temp_buffer), sizeof(buffer)))
                {
                    std::scoped_lock<std::mutex> lock(buffer_mutex);
                    std::memcpy(buffer, temp_buffer, sizeof(buffer));
                }
                else
                {
                    qDebug() << "ERROR: Client could not read the data from server";
                    connected = false;
                    serial.close();
                }
            }
            else
            {
                qDebug() << "Error: No data was sent in the time limit";
                connected = false;
                serial.close();
            }
        }
        else
        {
            if (serial.open(QIODevice::ReadOnly))
            {
                qDebug() << "Serial port opened";
                connected = true;
            }
            else
            {
                qDebug() << "Error: Client could not open the serial port";
                connected = false;
            }
        }

        msleep(20);
    }

    if (serial.isOpen())
    {
        serial.close();
    }
}

bool SerialPortService::get_connection_state()
{
    return connected && serial.isWritable();
}