#ifndef MODEL_H
#define MODEL_H

#include "iview.h"
#include "ialarm.h"

struct State
{
    double temperature = 101;
    double humidity = 101;
    bool lightIsOn = false;
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
