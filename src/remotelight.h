#ifndef REMOTELIGHT_H
#define REMOTELIGHT_H

#include "device.h"
#include "tcpconnection.h"

class RemoteLight : public Device
{
public:
    RemoteLight(unsigned short port);

    void turnOn() override;
    void turnOff() override;
    void toggle() override;

    bool isOn() override;

private:
    TcpConnection _connection;

    bool _isOn = false;
    void sendState();

    void messageReceived(char *data, int size);
};

#endif // REMOTELIGHT_H
