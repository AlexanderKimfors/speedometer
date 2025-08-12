#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "comservice.h"
#include <QSerialPort>
#include <thread>
#include <atomic>
#include <mutex>

class SerialPortService : public COMService
{

    std::atomic<bool> end{false};
    std::atomic<bool> connected{false};
    std::mutex buffer_mutex;
    QSerialPort serial;
    std::thread worker_thread;

    void run() override;

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

    bool get_connection_state();
};

#endif
