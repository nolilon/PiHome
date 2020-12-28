#ifndef MODEL_H
#define MODEL_H

#include "iview.h"
#include "ialarm.h"

struct State
{
    bool temperatureConnected = false;
    double temperature = 101;

    bool humidityConnected = false;
    double humidity = 101;

    bool lightConnected = false;
    bool lightIsOn = false;

    bool alarmConnected = false;
    Time alarmTime;
};


class Model
{
public:
    virtual State currentState() = 0;

    virtual void tempOrHumidPressed() = 0;
    virtual void toggleLight() = 0;
    virtual void setAlarmTime(Time newTime) = 0;
    virtual void stopAlarm() = 0;

    virtual ~Model() {}

    void setView(IView *view)
    {
        _view = view;
    }

protected:
    IView *_view = nullptr;
};

#endif // MODEL_H
