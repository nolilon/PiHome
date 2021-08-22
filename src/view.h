#ifndef VIEW_H
#define VIEW_H

#include "iview.h"
#include "telegramobjects.h"
#include <QTimer>

class Model;
class TelegramBot;
struct UpdatingMessage
{
    QString chat_id;
    QString message_id;
};

class View : public IView, public TelegramController
{
public:
    View(Model &model, TelegramBot &bot);

    void modelUpdated() override;

private:
    Model &_model;
    TelegramBot &_telegramBot;

    QList<UpdatingMessage> _messagesToUpdate;
    bool _messageIdsLoaded = true;

    QString const chat_id = "420638906";
    TelegramInlineKeyboard _keyboard;

    TelegramInlineButton *const _temperatureButton;
    TelegramInlineButton *const _humidityButton;
    TelegramInlineButton *const _lightButton;
    TelegramInlineButton *const _alarmButton;

    void loadUpdatingMessages();
    void update();
    QTimer _updateDelay;

    //CONTROLLER
    int acceptReply(const QString &reply) override;
};


#endif // VIEW_H
