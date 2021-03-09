#ifndef REMOTELIGHTALARM_H
#define REMOTELIGHTALARM_H

#include "ialarm.h"
#include "idevice.h"
#include "tcpconnection.h"
#include <QTimer>
#include <QTime>

class RemoteLightAlarm : public Device, public Alarm
{
public:
    explicit RemoteLightAlarm(unsigned short port);

    void setOnUpdated(OnUpdated onUpdated) override;

    void setTime(Time newTime) override;
    Time time() const override;
    void stop() override;

    void turnOn() override;
    void turnOff() override;
    void toggle() override;
    bool isOn() override;

    bool isConnected() override;

private:
    TcpConnection _connection;
    QTimer _timer;
    QTime _startTime;
    bool _isOn = false;
    OnUpdated _onUpdated = nullptr;

    QString const _alarmConfig = "alarm.ini";

    void timeCheck();
    void messageReceived(char *data, int size);
    void updated() const;
};

#endif // REMOTELIGHTALARM_H
