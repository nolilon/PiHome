#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "imodel.h"
#include <QTimer>

class Sensor;
class Device;

class MainLogic : public Model
{
public:
    MainLogic(Sensor *temperatureSensor,Sensor *humiditySensor, Device *light);

    State currentState() override;

    void tempOrHumidPressed() override;
    void toggleLight() override;

private:
    Sensor &_temperatureSensor;
    Sensor &_humiditySensor;
    Device &_light;

    QTimer _updateTimer;
};

#endif // MAINLOGIC_H
