#include "telegrambot.h"

#include "bottoken.h"
#include "telegramobjects.h"
#include "internet.h"

TelegramBot::TelegramBot(Internet &internet)
    : _internet(internet),
      _token(token)
{
    sendUpdatesRequest();
    _internet.setOnReply( [this] () {this->requestDone();} );

    _getUpdatesDelay.setInterval(1000);
    _getUpdatesDelay.setSingleShot(true);
    QObject::connect( &_getUpdatesDelay, &QTimer::timeout, [this] () {this->delayedGetUpdates();} );
    _getUpdatesDelay.start();
}

void TelegramBot::subscribeOnReply(TelegramController *controller)
{ _controller = controller; }

void TelegramBot::requestDone()
{
    if ( importantRequestDone() ) _importantRequest = false;
    else
    {
        const auto reply = _internet.reply();
        _lastUpdateId = _controller->acceptReply(reply);
    }

    if ( _importantRequest ) return;

    if ( _importantRequestsQueue.size() > 0 ) sendImportantRequest();
    else sendUpdatesRequest();
}

void TelegramBot::updateMessage(const TelegramObject &object)
{
    const QString method = "/editMessageText?";
    _importantRequestsQueue.push( _botApiUrl + _token + method + "message_id=57&" + object.json() );
    sendImportantRequest();
}

void TelegramBot::deleteMessage(int chat_id, int message_id)
{
    const QString method = "/deleteMessage?";
    _importantRequestsQueue.push( _botApiUrl +
                                  _token +
                                  method +
                                  "chat_id=" + QString::number(chat_id) +
                                  "&message_id=" + QString::number(message_id ) );
    sendImportantRequest();
}

void TelegramBot::sendMesage(const TelegramObject &object)
{
    const QString method = "/sendMessage?";
    _importantRequestsQueue.push( _botApiUrl + _token + method + object.json() );
    sendImportantRequest();
}

void TelegramBot::answerCallbackQuery(const QString &callbackQueryId)
{
    const QString method = "/answerCallbackQuery?callback_query_id=";
    _importantRequestsQueue.push( _botApiUrl + _token + method + callbackQueryId );
    sendImportantRequest();
}

bool TelegramBot::importantRequestDone() const
{ return _importantRequest; }

bool TelegramBot::updateReceived() const
{ return !_importantRequest; }

void TelegramBot::sendImportantRequest()
{
    if ( _importantRequest ) return;

    _internet.get( _importantRequestsQueue.front() );
    _importantRequestsQueue.pop();
    _importantRequest = true;
}

void TelegramBot::sendUpdatesRequest()
{
    _getUpdatesDelay.start();
}

void TelegramBot::delayedGetUpdates()
{
    if ( _importantRequest ) return;

    QString method = "/getUpdates?timeout=300&offset=" + QString::number(_lastUpdateId+1); //&allowed_updates=[\"callback_query\"]
    _internet.get(_botApiUrl + _token + method);
}
