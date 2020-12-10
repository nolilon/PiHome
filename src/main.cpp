#include "internet.h"
#include "telegrambot.h"
#include "remotewethersensors.h"
#include "mainlogic.h"
#include "view.h"
#include "ports.h"
#include <QCoreApplication>


int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    InternetPc internet;

    RemoteWeatherSensors weather(wetherPort);

    TelegramBot bot(internet);

    MainLogic logic(*(weather.tempSensor()), *(weather.humidSensor()));
    View view(logic, bot);

    a.exec();
}

