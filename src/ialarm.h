#ifndef IALARM_H
#define IALARM_H

struct Time
{
    int hours = 0;
    int minutes = 0;

    Time() {}
    Time(int h, int m)
        : hours(h),
          minutes(m)
    {}
};

class Alarm
{
public:
    virtual void setTime(Time newTime) = 0;
    virtual Time time() const = 0;
    virtual bool isConnected() = 0;
    virtual void stop() = 0;

    ~Alarm() {}
};

#endif // IALARM_H
