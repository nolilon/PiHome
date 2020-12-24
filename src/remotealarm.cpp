#include "remotealarm.h"

#include <QSettings>



RemoteAlarm::RemoteAlarm(unsigned short port)
    : _connection(port, [this] (char *data, int size) {this->messageReceived(data,size);} )
{
    QSettings config(_alarmConfig, QSettings::IniFormat);
    _startTime = config.value("startTime").toTime();

    _timer.setInterval(5000);
    QObject::connect( &_timer, &QTimer::timeout, [this] () {this->timeCheck();} );
    _timer.start();
}

void RemoteAlarm::setTime(Time newTime)
{
    _startTime = QTime(newTime.hours, newTime.minutes);
    QSettings config(_alarmConfig, QSettings::IniFormat);
    config.setValue("startTime", _startTime);
}

Time RemoteAlarm::time() const
{
    return Time(_startTime.hour(),_startTime.minute());
}

bool RemoteAlarm::isConnected()
{
    return true;
}

void RemoteAlarm::stop()
{
    char command = 'F';
    _connection.send(&command, 1);
}

void RemoteAlarm::timeCheck()
{
    auto currentTime = QTime::currentTime();
    auto lower = currentTime.addSecs(-3);
    auto upper = currentTime.addSecs(3);

    bool go = lower < _startTime && _startTime < upper;

    if ( go )
    {
        if ( _done ) return;
        else
        {
            char command = 'S';
            _connection.send(&command, 1);
            _done = true;
        }
    }
    else _done = false;
}

void RemoteAlarm::messageReceived(char *data, int size)
{}
