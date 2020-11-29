#ifndef SENSOR_H
#define SENSOR_H


class Sensor
{
public:
    virtual double value() = 0;

    virtual ~Sensor() {}
};

#endif // SENSOR_H
