#ifndef REMOTEALARM_H
#define REMOTEALARM_H

#include "ialarm.h"
#include "tcpconnection.h"
#include <QTimer>
#include <QTime>

class RemoteAlarm : public Alarm
{
public:
    RemoteAlarm(unsigned short port);

    void setTime(Time newTime) override;
    Time time() const override;
    bool isConnected() override;
    void stop() override;

private:
    TcpConnection _connection;
    QTimer _timer;
    QTime _startTime;
    bool _done = false;

    QString const _alarmConfig = "alarm.ini";

    void timeCheck();
    void messageReceived(char *data, int size);
};

#endif // REMOTEALARM_H
