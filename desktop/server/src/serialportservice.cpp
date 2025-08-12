#include "serialportservice.h"
#include <QDebug>

SerialPortService::SerialPortService()
{
    serial.setPortName(Settings::SerialPort::SERVER_PORT);
    serial.setBaudRate(Settings::SerialPort::BAUD_RATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);

    worker_thread = std::thread(&SerialPortService::run, this);
}

void SerialPortService::run()
{
    uint8_t last_sent[sizeof(buffer)]{0};

    while (!end)
    {
        if (connected && serial.isWritable())
        {
            uint8_t temp_buffer[sizeof(buffer)]{0};
            {
                std::scoped_lock<std::mutex> lock(buffer_mutex);
                std::memcpy(temp_buffer, buffer, sizeof(buffer));
            }

            if (std::memcmp(temp_buffer, last_sent, sizeof(buffer)) != 0)
            {
                std::memcpy(last_sent, temp_buffer, sizeof(buffer));
                QByteArray data(reinterpret_cast<const char *>(temp_buffer), sizeof(buffer));

                int bytesWritten = serial.write(data);

                serial.waitForBytesWritten(100);

                if (bytesWritten == -1)
                {
                    qDebug() << "Error: Failed to write to serial port:" << serial.errorString();
                    connected = false;
                    serial.close();
                }
                else if (bytesWritten != sizeof(buffer))
                {
                    qDebug() << "Error: Could not write all data to serial port:" << serial.errorString();
                    connected = false;
                    serial.close();
                }
                else
                {
                    qDebug() << "Successfully wrote all bytes to serial port.";
                }
            }
        }
        else
        {
            if (serial.open(QIODevice::ReadWrite))
            {
                qDebug() << "Serial port opened";
                connected = true;
            }
            else
            {
                qDebug() << " Error: Could not open the serial port in writeonly mood";
                connected = false;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

bool SerialPortService::get_connection_state()
{
    return connected && serial.isWritable();
}
