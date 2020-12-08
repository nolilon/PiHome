#include "remotewethersensors.h"


RemoteTemperature::RemoteTemperature(RemoteWeatherSensors *parent)
    : _parent(parent)
{}

double RemoteTemperature::value()
{
    return _parent->_lastTemp;
}

RemoteHumidit::RemoteHumidit(RemoteWeatherSensors *parent)
    : _parent(parent)
{}

double RemoteHumidit::value()
{
    return _parent->_lastHumid;
}

RemoteWeatherSensors::RemoteWeatherSensors(unsigned short port)
    : _temp(this),
      _humid(this),
      _connection( port, [this] (char *data, int size) {this->messageReceived(data,size);} )
{}

Sensor *RemoteWeatherSensors::tempSensor()
{
    return &_temp;
}

Sensor *RemoteWeatherSensors::humidSensor()
{
    return &_humid;
}

void RemoteWeatherSensors::messageReceived(char *data, int size)
{
    if (size != 4) return;

    short *temp = reinterpret_cast<short *>(data);
    short *humid = reinterpret_cast<short *>(data + 2);

    _lastTemp = (*temp)/10.;
    _lastHumid = (*humid)/10.;
}
