#ifndef VIEW_H
#define VIEW_H

#include "iview.h"
#include "telegramobjects.h"
#include <QTimer>

class Model;
class TelegramBot;

class View : public IView, public TelegramController
{
public:
    View(Model &model, TelegramBot &bot);

    void modelUpdated() override;
    void alarm(QString message) override;

private:
    Model &_model;
    TelegramBot &_telegramBot;

    TelegramInlineKeyboard _keyboard;

    TelegramInlineButton *const _temperatureButton;
    TelegramInlineButton *const _humidityButton;

    void update();
    QTimer _updateDelay;

    //CONTROLLER
    int acceptReply(const QString &reply) override;
};


#endif // VIEW_H
