#ifndef VIEW_H
#define VIEW_H

#include "iview.h"
#include "telegramobjects.h"

class Model;
class TelegramBot;

class View : public IView, public TelegramController
{
public:
    View(Model &model, TelegramBot &bot);

    void loop();

private:
    Model &_model;
    TelegramBot &_telegramBot;

    TelegramInlineKeyboard _keyboard;

    TelegramInlineButton *const _temperatureButton;
    TelegramInlineButton *const _humidityButton;

    bool _needUpdate = true;

    void modelUpdated() override;
    void alarm(QString message) override;

    //CONTROLLER
    int acceptReply(const QString &reply) override;
};


#endif // VIEW_H
