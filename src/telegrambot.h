#ifndef TELEGRAMBOT_H
#define TELEGRAMBOT_H

#include <QString>
#include <queue>

class TelegramController;
class TelegramObject;
class Internet;

class TelegramBot
{
public:
    explicit TelegramBot(Internet &internet);
    void subscribeOnReply(TelegramController *controller);

    void loop();

    void sendMesage(const TelegramObject &object);
    void updateMessage(const TelegramObject &object);
    void answerCallbackQuery(const QString &callbackQueryId);

private:
    Internet &_internet;
    const QString _botApiUrl = "https://api.telegram.org/bot";
    const QString _token;

    TelegramController *_controller = nullptr;

    long _lastUpdateId = 0;

    std::queue<QString> _importantRequestsQueue;
    bool _importantRequest = false;
    bool importantRequestInProgress() const;
    bool importantRequestDone() const;
    bool updateReceived() const;

    void sendImportantRequest();
    void sendUpdatesRequest();
};


#endif // TELEGRAMBOT_H
