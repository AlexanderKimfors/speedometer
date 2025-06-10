#include "tcpservice.h"
#include <QAbstractSocket>

TCPService::TCPService()
{
    QObject::connect(&socket_, &QTcpSocket::readyRead, [this]()
                     { handleReadyRead(); });
}

bool TCPService::connectToHost(const QString &host, quint16 port)
{
    socket_.connectToHost(host, port);
    return socket_.waitForConnected(3000); // Optional timeout (3 seconds)
}

void TCPService::disconnectFromHost()
{
    socket_.disconnectFromHost();
}

bool TCPService::isConnected() const
{
    return socket_.state() == QAbstractSocket::ConnectedState;
}

bool TCPService::sendData(const QByteArray &data)
{
    if (!isConnected())
        return false;
    qint64 bytesWritten = socket_.write(data);
    return bytesWritten == data.size();
}

QByteArray TCPService::receiveData()
{
    newDataAvailable_ = false;
    QByteArray result = buffer_;
    buffer_.clear();
    return result;
}

bool TCPService::hasNewData() const
{
    return newDataAvailable_;
}

void TCPService::handleReadyRead()
{
    buffer_.append(socket_.readAll());
    newDataAvailable_ = true;
}
