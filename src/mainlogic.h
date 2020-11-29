#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include "imodel.h"
#include <QTimer>

class Sensor;

class MainLogic : public Model
{
public:
    MainLogic(Sensor &temperatureSensor,Sensor &humiditySensor);

    void loop();
    State currentState() override;

    void tempOrHumidPressed() override;

private:
    Sensor &_temperatureSensor;
    Sensor &_humiditySensor;

    QTimer _updateTimer;
};

#endif // MAINLOGIC_H
