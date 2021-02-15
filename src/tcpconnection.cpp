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
    QObject::connect( _tcpServer, &QTcpServer::acceptError, [this] () {this->onServerError();} );
    QObject::connect( _tcpServer, &QTcpServer::newConnection, [this]() {this->clientConnecting();} );
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
    auto state = _client->state() == QAbstractSocket::ConnectedState;
    if (!state) qWarning() << "Client on port " << _port << " state: " << _client->state();
    return state;
}

void TcpConnection::listenPort()
{
    qInfo() << "Trying to listen " << _port << " port";
    _tcpServer->listen(QHostAddress::AnyIPv4, _port);
    bool success = _tcpServer->isListening();
    if (success) qInfo() << "Listening port " << _port << "\n";
    else
    {
        qWarning() << "Port is not opened!!";
        QTimer::singleShot(30000, [this] () {this->listenPort();});
    }
}

void TcpConnection::clientConnecting()
{
    auto client = _tcpServer->nextPendingConnection();
    if ( _tcpServer->hasPendingConnections() ) qWarning() << "Port " << _port << "still has pending connections!";
    if ( _client != nullptr ) delete _client;
    _client = client;
    QObject::connect( _client, &QTcpSocket::errorOccurred, [this] () {this->onClientError();} );
    QObject::connect( _client, &QTcpSocket::readyRead, [this]() {this->readMessage();} );
    qInfo() << "Client connected on " << _client->localPort() << " port from " << _client->localAddress();
}

void TcpConnection::readMessage()
{
    auto data = _client->readAll();
    _callback(data.data(), data.size());
}

void TcpConnection::onServerError()
{
    qWarning() << "Server on port " << _port << "has error: " << _tcpServer->errorString();
}

void TcpConnection::onClientError()
{
    qWarning() << "Client on port " << _port << "has error: " << _client->errorString();
}
