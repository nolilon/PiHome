#ifndef TELEGRAMOBJECTS_H
#define TELEGRAMOBJECTS_H

#include <QString>
#include <QList>
#include <functional>

typedef int LastUpdateId;

class TelegramController
{
public:
    virtual LastUpdateId acceptReply(const QString &reply) = 0;
    virtual ~TelegramController() {}
};

typedef std::function<void()> TelegramCallback;



class TelegramObject
{
public:
    virtual QString json() const = 0;
    virtual ~TelegramObject() {};
};


class TelegramMessage : public TelegramObject
{
public:
    explicit TelegramMessage(QString text); //CRUTCH
    TelegramMessage(QString text, QString chatId);
    QString json() const override;

private:
    const QString _text;
    const QString _chat_id = "420638906"; //CRUTCH
};



class TelegramComplexMessage : public TelegramObject
{
public:
    TelegramComplexMessage(const TelegramMessage &textMessage, const TelegramObject &optionalParameter);
    QString json() const override;

private:
    const TelegramMessage &_textMessage;
    const TelegramObject &_optionalParameter;
};



class TelegramInlineButton : public TelegramObject
{
public:
    TelegramInlineButton(TelegramCallback callback, QString callbackData);

    void updateText(QString text);
    void doCallback() const;
    QString json() const override;

private:
    QString _text;
    const TelegramCallback _callBack;
    const QString _callbackData;
};


class QJsonObject;

class TelegramInlineKeyboard : public TelegramObject
{
public:
    TelegramInlineKeyboard();
    ~TelegramInlineKeyboard();
    TelegramInlineButton *createButton(TelegramCallback callback);

    QString json() const override;

    void checkReply(const QJsonObject &callback_query);


private:
    QList<TelegramInlineButton*> _buttons;
};



//class TelegramReplyButton : public TelegramObject
//{
//public:
//    explicit TelegramReplyButton(QString text);
//    QString json() const override;

//private:
//    const QString _text;
//};



//template used instead dynamic array
//template<int size>
//class TelegramReplyKeyboard : public TelegramObject
//{
//public:
//    TelegramReplyKeyboard() {}
//    void addButton(TelegramReplyButton *button)
//    {
//        for (int i = 0; i < size; ++i)
//        {
//            if ( _buttons[i] != nullptr ) continue;

//            _buttons[i] = button;
//            break;
//        }
//    }

//    QString json() const override
//    {
//        QString result = "reply_markup={\"keyboard\":[";

//        for (int i = 0; i < size; ++i)
//        {
//            result = result + "[" + _buttons[i]->json() + "]";

//            if ( i != size-1 )
//            {
//                result = result + ",";
//            }
//        }
//        result = result + "]}";
//        return result;
//    }

//private:
//    TelegramReplyButton *_buttons[size] = {nullptr};
//};

#endif // TELEGRAMOBJECTS_H
