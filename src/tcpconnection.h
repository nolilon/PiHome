#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <functional>
typedef std::function<void(char *, int)> OnMessageCallback;

class QTcpServer;
class QTcpSocket;

class TcpConnection
{
public:
    TcpConnection(unsigned short port, OnMessageCallback callback);
    void send(char *data, int size);

    bool isConnected() const;

private:
    void clientConnecting();

    void readMessage();
    OnMessageCallback const _callback;

    QTcpServer *const _tcpServer;
    QTcpSocket *_client = nullptr;
};

#endif // TCPCONNECTION_H
