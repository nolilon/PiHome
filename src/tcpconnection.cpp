#include "tcpconnection.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>


TcpConnection::TcpConnection(quint16 port, OnMessageCallback callback)
    : _port(port),
      _callback(callback),
      _tcpServer(new QTcpServer)
{
    QObject::connect( _tcpServer, &QTcpServer::acceptError, this, &TcpConnection::onError );
    listenPort();
}

void TcpConnection::send(char *data, int size)
{
    if ( _client == nullptr ) return;
    _client->write(data,size);
}

bool TcpConnection::isConnected() const
{
    if ( _client == nullptr ) return false;
    return _client->state() == QAbstractSocket::ConnectedState;
}

void TcpConnection::listenPort()
{
    QObject::connect( _tcpServer, &QTcpServer::newConnection, [this]() {this->clientConnecting();} );
    _tcpServer->listen(QHostAddress::AnyIPv4, _port);
    bool success = _tcpServer->isListening();
    if (success) qDebug() << "Listening port " << _port << "\n";
    else
    {
        qDebug() << "Port not opened!!";
        QTimer::singleShot(30000, this, &TcpConnection::listenPort);
    }
}

void TcpConnection::clientConnecting()
{
    auto client = _tcpServer->nextPendingConnection();
    if ( _client != nullptr ) delete _client;
    _client = client;
    QObject::connect( _client, &QTcpSocket::readyRead, [this]() {this->readMessage();} );
    qDebug() << "Client connected on " << _client->localPort() << " port";
}

void TcpConnection::readMessage()
{
    auto data = _client->readAll();
    _callback(data.data(), data.size());
}

void TcpConnection::onError()
{
    qDebug() << _tcpServer->errorString();
}
