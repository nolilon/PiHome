#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "imodel.h"
#include <QTimer>

class Sensor;
class Device;
class Alarm;

class MainLogic : public Model
{
public:
    MainLogic(Sensor *temperatureSensor,Sensor *humiditySensor, Device *light, Alarm *alarm);

    State currentState() override;

    void tempOrHumidPressed() override;
    void toggleLight() override;
    void setAlarmTime(Time newTime) override;
    void stopAlarm() override;

private:
    Sensor &_temperatureSensor;
    Sensor &_humiditySensor;
    Device &_light;
    Alarm &_alarm;

    QTimer _updateTimer;
};

#endif // MAINLOGIC_H
