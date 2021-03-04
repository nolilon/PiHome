#include "remotelightalarm.h"

#include <QSettings>

RemoteLightAlarm::RemoteLightAlarm(unsigned short port)
    : _connection(port, [this] (char *data, int size) {this->messageReceived(data,size);} )
{
    QSettings config(_alarmConfig, QSettings::IniFormat);
    _startTime = config.value("startTime").toTime();

    _timer.setInterval(5000);
    QObject::connect( &_timer, &QTimer::timeout, [this] () {this->timeCheck();} );
    _timer.start();
}

void RemoteLightAlarm::setTime(Time newTime)
{
    _startTime = QTime(newTime.hours, newTime.minutes);
    QSettings config(_alarmConfig, QSettings::IniFormat);
    config.setValue("startTime", _startTime.toString());
}

Time RemoteLightAlarm::time() const
{
    return Time(_startTime.hour(),_startTime.minute());
}

void RemoteLightAlarm::timeCheck()
{
    auto currentTime = QTime::currentTime();
    auto lower = currentTime.addSecs(-3);
    auto upper = currentTime.addSecs(3);

    bool go = lower < _startTime && _startTime < upper;

    if ( go )
    {
        if ( _isOn ) return;
        else
        {
            char command = 'S';
            _connection.send(&command, 1);
        }
    }
}

void RemoteLightAlarm::stop()
{
    char command = 'F';
    _connection.send(&command, 1);
}



void RemoteLightAlarm::turnOn()
{
    if (_isOn) return;
    char command = 'T';
    _connection.send(&command, 1);
}

void RemoteLightAlarm::turnOff()
{
    if (!_isOn) return;
    char command = 'T';
    _connection.send(&command, 1);
}

void RemoteLightAlarm::toggle()
{
    char command = 'T';
    _connection.send(&command, 1);
}

bool RemoteLightAlarm::isOn()
{
    return _isOn;
}


bool RemoteLightAlarm::isConnected()
{
    return _connection.isConnected();
}

void RemoteLightAlarm::messageReceived(char *data, int size)
{
    _isOn = data[size-1];
}
