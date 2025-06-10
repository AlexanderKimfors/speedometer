#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <QString>
#include <QByteArray>

class COMService
{
public:
    virtual ~COMService() = default;

    virtual bool connectToHost(const QString &host, quint16 port) = 0;
    virtual void disconnectFromHost() = 0;
    virtual bool isConnected() const = 0;

    virtual bool sendData(const QByteArray &data) = 0;
    virtual QByteArray receiveData() = 0;
};

#endif