#ifndef TCPCOM_H
#define TCPCOM_H

#include "comservice.h"
#include <QTcpSocket>

class TCPService : public COMService
{
public:
    TCPService();

    bool connectToHost(const QString &host, quint16 port) override;
    void disconnectFromHost() override;
    bool isConnected() const override;

    bool sendData(const QByteArray &data) override;
    QByteArray receiveData() override;

    // Manual polling alternative to signals
    bool hasNewData() const;

private:
    void handleReadyRead();

    QTcpSocket socket_;
    QByteArray buffer_;
    bool newDataAvailable_ = false;
};

#endif