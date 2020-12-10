#include "mainlogic.h"

#include "isensor.h"


MainLogic::MainLogic(Sensor &temperatureSensor, Sensor &humiditySensor)
    : _temperatureSensor(temperatureSensor),
      _humiditySensor(humiditySensor)
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
    return result;
}

void MainLogic::tempOrHumidPressed()
{
    _view->modelUpdated();
}
