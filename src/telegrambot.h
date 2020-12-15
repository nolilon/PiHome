#ifndef TELEGRAMBOT_H
#define TELEGRAMBOT_H

#include <QString>
#include <queue>
#include <QTimer>

class TelegramController;
class TelegramObject;
class Internet;

class TelegramBot
{
public:
    explicit TelegramBot(Internet &internet);
    void subscribeOnReply(TelegramController *controller);


    void sendMesage(const TelegramObject &object);
    void updateMessage(const TelegramObject &object);
    void deleteMessage(int chat_id, int message_id);
    void answerCallbackQuery(const QString &callbackQueryId);

private:
    Internet &_internet;
    const QString _botApiUrl = "https://api.telegram.org/bot";
    const QString _token;

    TelegramController *_controller = nullptr;

    void requestDone();

    long _lastUpdateId = 0;

    std::queue<QString> _importantRequestsQueue;
    bool _importantRequest = false;
    bool importantRequestDone() const;
    bool updateReceived() const;

    void sendImportantRequest();
    void sendUpdatesRequest();
    QTimer _getUpdatesDelay;
    void delayedGetUpdates();
};


#endif // TELEGRAMBOT_H
