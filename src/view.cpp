#include "view.h"

#include "imodel.h"
#include "telegrambot.h"
#include "telegramobjects.h"

#include <QDateTime>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


View::View(Model &model, TelegramBot &bot)
    : _model(model),
      _telegramBot(bot),
      _temperatureButton( _keyboard.createButton( std::bind(&Model::tempOrHumidPressed, &_model) ) ),
      _humidityButton( _keyboard.createButton( std::bind(&Model::tempOrHumidPressed, &_model) ) )
{
    _model.setView(this);
    _telegramBot.subscribeOnReply(this);
}

void View::loop()
{
    _telegramBot.loop();

    if ( !_needUpdate ) return;

    auto state = _model.currentState();

    auto currentDateTime = QDateTime::currentDateTime().toString("d MMMM yyyy, h:mm ap");
    TelegramMessage textMessage("All is OK\n" + currentDateTime);

    _temperatureButton->updateText( QString::asprintf("   Temperature:       %.1f   ", state.temperature) );
    _humidityButton->updateText(    QString::asprintf("   Humidity:             %.1f   ", state.humidity) );

    TelegramComplexMessage message( textMessage, _keyboard );
    _telegramBot.updateMessage(message);

    _needUpdate = false;
}

void View::modelUpdated()
{
    _needUpdate = true;
}

void View::alarm(QString message)
{
    Q_UNUSED(message)
}


//------------- CONTROLLER --------------//
int View::acceptReply(const QString &reply)
{
    QElapsedTimer timer;
    timer.start();
    long lastUpdateId = 0;
    auto json = QJsonDocument::fromJson( reply.toUtf8() ); //the most long operation, ~100mksec
    auto updateList = json["result"].toArray();

    for (const QJsonValue &update : updateList)
    {
        auto update_id = update["update_id"].toInt(0);

        auto callback_query = update["callback_query"].toObject();
        _keyboard.checkReply( callback_query );

        auto callbackQueryId = callback_query["id"].toString();
        _telegramBot.answerCallbackQuery(callbackQueryId);
        lastUpdateId = update_id;
    }
    qDebug() << "Answer takes " << timer.nsecsElapsed() << "\n";
    return lastUpdateId;
}
