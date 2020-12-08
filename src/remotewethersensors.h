#ifndef REMOTEWETHERSENSORS_H
#define REMOTEWETHERSENSORS_H

#include "isensor.h"


class RemoteWeatherSensors;

class RemoteTemperature : public Sensor
{
public:
    RemoteTemperature(RemoteWeatherSensors *parent);

    double value() override;

private:
    RemoteWeatherSensors *const _parent;
};



class RemoteHumidit : public Sensor
{
public:
    RemoteHumidit(RemoteWeatherSensors *parent);

    double value() override;

private:
    RemoteWeatherSensors *const _parent;
};


#include "tcpconnection.h"

class RemoteWeatherSensors
{
public:
    RemoteWeatherSensors(unsigned short port);

    Sensor *tempSensor();
    Sensor *humidSensor();

private:
    double _lastTemp = -1;
    double _lastHumid = -1;

    friend RemoteTemperature;
    friend RemoteHumidit;

    RemoteTemperature _temp;
    RemoteHumidit _humid;

    TcpConnection _connection;
    void messageReceived(char *data, int size);
};

#endif // REMOTEWETHERSENSORS_H
