#include "internet.h"
#include "telegrambot.h"
#include "remotewethersensors.h"
#include "mainlogic.h"
#include "view.h"
#include <QCoreApplication>


int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    InternetPc internet;

    RemoteTemperature temperatureSensor;
    RemoteHumidit humiditySensor;
    
    TelegramBot bot(internet);

    MainLogic logic(temperatureSensor, humiditySensor);
    View view(logic, bot);

    while (true)
    {
        logic.loop();
        view.loop();
    }
}

