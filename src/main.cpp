#include "internet.h"
#include "telegrambot.h"
#include "mainlogic.h"
#include "view.h"
#include "ports.h"

#include "remotewethersensors.h"
#include "remotelightalarm.h"

#include <QCoreApplication>
#include <QFile>
const QString logFileName = "log.txt";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    QFile::remove(logFileName);
    qInstallMessageHandler(myMessageOutput);

    InternetPc internet;

    RemoteWeatherSensors weather(wetherPort);
    RemoteLightAlarm lightAlarm(alarmPort);

    TelegramBot bot(internet);

    MainLogic logic(weather.tempSensor(),
                    weather.humidSensor(),
                    &lightAlarm,
                    &lightAlarm);
    View view(logic, bot);

    a.exec();
}


#include <QDateTime>
#include <QMutex>
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();

    static QMutex mutex;
    QFile outFile(logFileName);

    mutex.lock();
    outFile.open(QIODevice::Append);
    QTextStream ts(&outFile);
    ts << QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss");

    switch (type) {
    case QtDebugMsg:
        ts << " D: " + msg + '\n';
        break;
    case QtInfoMsg:
        ts << " I: " + msg + '\n';
        break;
    case QtWarningMsg:
        ts << " W: " + msg + '\n';
        break;
    case QtCriticalMsg:
        ts << " C: " + msg + '\n';
        break;
    case QtFatalMsg:
        ts << " Fatal: " + msg + '\n';
        mutex.unlock();
        abort();
        break;
    }
    outFile.close();
    mutex.unlock();
}
