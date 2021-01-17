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
      _humidityButton( _keyboard.createButton( std::bind(&Model::tempOrHumidPressed, &_model) ) ),
      _lightButton( _keyboard.createButton( std::bind(&Model::toggleLight, &_model) ) ),
      _alarmButton( _keyboard.createButton( std::bind(&Model::stopAlarm, &_model) ) )
{
    _model.setView(this);
    _telegramBot.subscribeOnReply(this);

    _updateDelay.setSingleShot(true);
    _updateDelay.setInterval(10);
    QObject::connect( &_updateDelay, &QTimer::timeout, [this] () {this->update();} );
    _updateDelay.start();
}

void View::update()
{
    auto state = _model.currentState();

    auto currentDateTime = QDateTime::currentDateTime().toString("d MMMM yyyy, h:mm ap");
    TelegramMessage textMessage("Last update:\n" + currentDateTime, chat_id);

    QString tempMessage;
    if (state.temperatureConnected) tempMessage = QString::asprintf("   Temperature:       %.1f   ", state.temperature);
    else tempMessage = "   Temperature:       n/c   ";
    _temperatureButton->updateText( tempMessage );

    QString humidMessage;
    if (state.humidityConnected) humidMessage = QString::asprintf("   Humidity:             %.1f   ", state.humidity);
    else humidMessage = "   Humidity:             n/c   ";
        _humidityButton->updateText( humidMessage );

    QString lightState;
    if (!state.lightConnected) lightState = "n/c";
    else if (state.lightIsOn) lightState = "On";
    else lightState = "Off";
    _lightButton->updateText( QString("   Light:                  %1   ").arg(lightState) );

    QString alarmMessage;
    if (state.alarmConnected) alarmMessage = QString("   Alarm:              %1:%2").arg(state.alarmTime.hours).arg(state.alarmTime.minutes, 2, 10, QChar('0'));
    else alarmMessage = "   Alarm:              n/c";
    _alarmButton->updateText( alarmMessage );

    TelegramComplexMessage message( textMessage, _keyboard );
    _telegramBot.updateMessage(message);
}

void View::modelUpdated()
{
    _updateDelay.start();
}



//------------- CONTROLLER --------------//
int View::acceptReply(const QString &reply)
{
//    QElapsedTimer timer;
//    timer.start();
//    long long elapsed = 0;

    long lastUpdateId = 0;
    auto json = QJsonDocument::fromJson( reply.toUtf8() ); //the most long operation, ~100mksec
    auto updateList = json["result"].toArray();

    for (const QJsonValue &update : updateList)
    {
        auto update_id = update["update_id"].toInt(0);
        auto updateJson = update.toObject();

        if ( updateJson.contains("callback_query") )
        {
            auto callback_query = update["callback_query"].toObject();
            _keyboard.checkReply( callback_query );

            auto callbackQueryId = callback_query["id"].toString();
            _telegramBot.answerCallbackQuery(callbackQueryId);
        }
        else if ( updateJson.contains("message") )
        {
            auto message = updateJson["message"].toObject();
            auto messageId = message["message_id"].toInt();
            auto chatId = message["chat"].toObject()["id"].toInt();
            _telegramBot.deleteMessage(chatId, messageId);

            auto text = message["text"].toString();
            auto command = text.section(' ',0,0);
            if (command == "Alarm" || command == "alarm")
            {
                QTime time = QTime::fromString(text.section(' ',1,1),"h:mm");
                if ( time.isValid() ) _model.setAlarmTime( Time(time.hour(),time.minute()) );
            }
        }

        lastUpdateId = update_id;
    }
//    elapsed = timer.nsecsElapsed();
//    qDebug() << "Answer takes " << elapsed << "\n";
    return lastUpdateId;
}
