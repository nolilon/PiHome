#ifndef MODEL_H
#define MODEL_H

#include "iview.h"

struct State
{
    double temperature = 101;
    double humidity = 101;
};


class Model
{
public:
    virtual State currentState() = 0;

    virtual void tempOrHumidPressed() = 0;

    virtual ~Model() {}

    void setView(IView *view)
    {
        _view = view;
    }

protected:
    IView *_view = nullptr;
};

#endif // MODEL_H
