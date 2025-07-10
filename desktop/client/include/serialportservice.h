#ifndef SERIALPORTSERVICE_H
#define SERIALPORTSERVICE_H

#include "comservice.h"
#include <QSerialPort>
#include <thread>

class SerialPortService : public COMService
{
private:
    std::atomic<bool> connected{false};
    std::atomic<bool> end{false};
    QSerialPort serial;
    std::thread worker_thread;

    void run();

public:
    SerialPortService();
    ~SerialPortService()
    {
        end = true;
        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
    }

    bool get_connection_state() override;
};

#endif