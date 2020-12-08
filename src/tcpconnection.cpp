#include "tcpconnection.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>


TcpConnection::TcpConnection(quint16 port, OnMessageCallback callback)
    : _callback(callback),
      _tcpServer(new QTcpServer)
{
    QObject::connect( _tcpServer, &QTcpServer::newConnection, [this]() {this->clientConnecting();} );
    _tcpServer->listen(QHostAddress::Any, port);
    bool success = _tcpServer->isListening();
    if (success) qDebug() << "Listening port " << port << "\n";
    else qDebug() << "Port not opened!!";
}

void TcpConnection::clientConnecting()
{
    auto client = _tcpServer->nextPendingConnection();
    if ( _client != nullptr ) delete _client;
    _client = client;
    QObject::connect( _client, &QTcpSocket::readyRead, [this]() {this->readMessage();} );
}

void TcpConnection::readMessage()
{
    auto data = _client->readAll();
    _callback(data.data(), data.size());
}
