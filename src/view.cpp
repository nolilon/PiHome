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
#include <QSettings>


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
    loadUpdatingMessages();

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

    QString tempMessage = QString::asprintf("   Temperature:       %.1f   ", state.temperature);
    if (!state.temperatureConnected) tempMessage = '-' + tempMessage;
    _temperatureButton->updateText( tempMessage );

    QString humidMessage = QString::asprintf("   Humidity:             %.1f   ", state.humidity);
    if (!state.humidityConnected) humidMessage = '-' + humidMessage;
    _humidityButton->updateText( humidMessage );

    QString lightMessage = QString("   Light:                  %1   ");
    if (state.lightIsOn) lightMessage = lightMessage.arg( "On" );
    else lightMessage = lightMessage.arg( "Off" );
    if (!state.lightConnected) lightMessage = '-' + lightMessage;
    _lightButton->updateText( lightMessage );

    QString alarmMessage = QString("   Alarm:              %1:%2").arg(state.alarmTime.hours).arg(state.alarmTime.minutes, 2, 10, QChar('0'));
    if (!state.alarmConnected) alarmMessage = '-' + alarmMessage;
    _alarmButton->updateText( alarmMessage );

    TelegramComplexMessage message( textMessage, _keyboard );
    _telegramBot.updateMessage(message, "57");
}

void View::modelUpdated()
{
    _updateDelay.start();
}

void View::loadUpdatingMessages()
{
    QSettings messages("updatingMessages.ini", QSettings::IniFormat);
    const auto groups = messages.childGroups();
    for (const auto &group : groups)
    {
        messages.beginGroup(group);
        UpdatingMessage message;
        message.chat_id = messages.value("chat_id").toString();
        message.message_id = messages.value("message_id").toString();
        messages.endGroup();
        if (message.message_id.isEmpty()) _messageIdsLoaded = false;
        _messagesToUpdate.append(message);
    }
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
            auto messageId = message["message_id"].toVariant().toString();
            auto chatId = message["chat"].toObject()["id"].toVariant().toString();
            if (chatId == chat_id)
            {
                _telegramBot.deleteMessage(chatId, messageId);

                auto text = message["text"].toString();
                auto command = text.section(' ',0,0);
                if (command == "Alarm" || command == "alarm")
                {
                    QTime time = QTime::fromString(text.section(' ',1,1),"h:mm");
                    if ( time.isValid() ) _model.setAlarmTime( Time(time.hour(),time.minute()) );
                }
            }
        }

        lastUpdateId = update_id;
    }
//    elapsed = timer.nsecsElapsed();
//    qDebug() << "Answer takes " << elapsed << "\n";
    return lastUpdateId;
}
