#include "mainlogic.h"

#include "isensor.h"
#include "device.h"


MainLogic::MainLogic(Sensor *temperatureSensor, Sensor *humiditySensor, Device *light, Alarm *alarm)
    : _temperatureSensor(*temperatureSensor),
      _humiditySensor(*humiditySensor),
      _light(*light),
      _alarm(*alarm)
{
    _updateTimer.setInterval(600000);
    _updateTimer.start();
    QObject::connect( &_updateTimer, &QTimer::timeout, [this] () {this->_view->modelUpdated();} );
}

State MainLogic::currentState()
{
    _updateTimer.start();

    State result;
    result.temperature = _temperatureSensor.value();
    result.humidity = _humiditySensor.value();
    result.lightIsOn = _light.isOn();
    result.alarmTime = _alarm.time();
    return result;
}

void MainLogic::tempOrHumidPressed()
{
    _view->modelUpdated();
}

void MainLogic::toggleLight()
{
    _light.toggle();
    _view->modelUpdated();
}

void MainLogic::setAlarmTime(Time newTime)
{
    _alarm.setTime(newTime);
    _view->modelUpdated();
}

void MainLogic::stopAlarm()
{
    _alarm.stop();
}
