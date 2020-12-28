#include "remotelight.h"

RemoteLight::RemoteLight(unsigned short port)
    : _connection(port, [this] (char *data, int size) {messageReceived(data, size);} )
{}

void RemoteLight::turnOn()
{
    _isOn = true;
    sendState();
}

void RemoteLight::turnOff()
{
    _isOn = false;
    sendState();
}

void RemoteLight::toggle()
{
    _isOn = !_isOn;
    sendState();
}

bool RemoteLight::isOn()
{
    return _isOn;
}

bool RemoteLight::isConnected()
{
    return _connection.isConnected();
}

void RemoteLight::sendState()
{
    char command;

    if (_isOn) command = 'U';
    else command = 'D';

    _connection.send(&command, 1);
}

void RemoteLight::messageReceived(char *data, int size)
{
    _isOn = data[size-1];
}
