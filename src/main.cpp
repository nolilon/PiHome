#include "internet.h"
#include "telegrambot.h"
#include "mainlogic.h"
#include "view.h"
#include "ports.h"
#include <QCoreApplication>

#include "remotewethersensors.h"
#include "remotealarm.h"
#include "remotelight.h"


int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    InternetPc internet;

    RemoteWeatherSensors weather(wetherPort);
    RemoteAlarm alarm(alarmPort);
    RemoteLight light(lightPort);

    TelegramBot bot(internet);

    MainLogic logic(weather.tempSensor(),
                    weather.humidSensor(),
                    &light);
    View view(logic, bot);

    a.exec();
}

