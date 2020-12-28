#ifndef DEVICE_H
#define DEVICE_H


class Device
{
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void toggle() = 0;

    virtual bool isOn() = 0;
    virtual bool isConnected() = 0;

    virtual ~Device() {}
};

#endif // DEVICE_H
