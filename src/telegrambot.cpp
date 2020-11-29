#include "telegrambot.h"

#include "bottoken.h"
#include "telegramobjects.h"
#include "internet.h"

TelegramBot::TelegramBot(Internet &internet)
    : _internet(internet),
      _token(token)
{}

void TelegramBot::subscribeOnReply(TelegramController *controller)
{ _controller = controller; }

void TelegramBot::loop()
{
    _internet.loop();

    if ( importantRequestInProgress() ) return;
    else if ( importantRequestDone() ) _importantRequest = false;
    else if ( updateReceived() )
    {
        const auto reply = _internet.reply();
        _lastUpdateId = _controller->acceptReply(reply);
    }

    if ( _importantRequestsQueue.size() > 0 ) sendImportantRequest();
    else if ( _internet.replyReady() ) sendUpdatesRequest();
}

void TelegramBot::updateMessage(const TelegramObject &object)
{
    const QString method = "/editMessageText?";
    _importantRequestsQueue.push( _botApiUrl + _token + method + "message_id=57&" + object.json() );
}

void TelegramBot::sendMesage(const TelegramObject &object)
{
    const QString method = "/sendMessage?";
    _importantRequestsQueue.push( _botApiUrl + _token + method + object.json() );
}

void TelegramBot::answerCallbackQuery(const QString &callbackQueryId)
{
    const QString method = "/answerCallbackQuery?callback_query_id=";
    _importantRequestsQueue.push( _botApiUrl + _token + method + callbackQueryId );
}

bool TelegramBot::importantRequestInProgress() const
{ return _importantRequest && !_internet.replyReady(); }

bool TelegramBot::importantRequestDone() const
{ return _importantRequest && _internet.replyReady(); }

bool TelegramBot::updateReceived() const
{ return !_importantRequest && _internet.replyReady(); }

void TelegramBot::sendImportantRequest()
{
    _internet.get( _importantRequestsQueue.front() );
    _importantRequestsQueue.pop();
    _importantRequest = true;
}

void TelegramBot::sendUpdatesRequest()
{
    QString method = "/getUpdates?timeout=300&allowed_updates=[\"callback_query\"]&offset=" + QString::number(_lastUpdateId+1);
    _internet.get(_botApiUrl + _token + method);
}
