#ifndef REMOTEWETHERSENSORS_H
#define REMOTEWETHERSENSORS_H

#include "isensor.h"


class RemoteTemperature : public Sensor
{
public:
    RemoteTemperature();

    double value() override;
};



class RemoteHumidit : public Sensor
{
public:
    RemoteHumidit();

    double value() override;
};

#endif // REMOTEWETHERSENSORS_H
