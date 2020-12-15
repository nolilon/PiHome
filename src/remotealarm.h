#ifndef REMOTEALARM_H
#define REMOTEALARM_H

#include "tcpconnection.h"
#include <QTimer>
#include <QTime>

class RemoteAlarm
{
public:
    RemoteAlarm(unsigned short port);

private:
    TcpConnection _connection;
    QTimer _timer;
    QTime _startTime;
    bool _done = false;

    void timeCheck();
    void messageReceived(char *data, int size);
};

#endif // REMOTEALARM_H
