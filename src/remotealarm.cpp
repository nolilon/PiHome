#include "remotealarm.h"

RemoteAlarm::RemoteAlarm(unsigned short port)
    : _connection(port, [this] (char *data, int size) {this->messageReceived(data,size);} ),
      _startTime(8,45)
{
    _timer.setInterval(5000);
    QObject::connect( &_timer, &QTimer::timeout, [this] () {this->timeCheck();} );
    _timer.start();
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
