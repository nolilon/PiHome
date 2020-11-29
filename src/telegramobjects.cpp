#include "telegramobjects.h"

#include <QJsonObject>


TelegramMessage::TelegramMessage(QString text)
    : _text(text)
{}

TelegramMessage::TelegramMessage(QString text, QString chatId)
    : _text(text),
      _chat_id(chatId)
{}

QString TelegramMessage::json() const
{
    return "chat_id=" + _chat_id + "&text=" + _text;
}


TelegramComplexMessage::TelegramComplexMessage(const TelegramMessage &textMessage, const TelegramObject &optionalParameter)
    : _textMessage(textMessage),
      _optionalParameter(optionalParameter)
{}

QString TelegramComplexMessage::json() const
{
    return _textMessage.json() + "&" + _optionalParameter.json();
}



TelegramInlineButton::TelegramInlineButton(TelegramCallback callback, QString callbackData)
    : _callBack(callback),
      _callbackData(callbackData)
{}

void TelegramInlineButton::updateText(QString text)
{
    _text = text;
}

void TelegramInlineButton::doCallback() const
{
    _callBack();
}

QString TelegramInlineButton::json() const
{
    return "{\"text\":\"" + _text + "\",\"callback_data\":" + _callbackData + "}";
}


TelegramInlineKeyboard::TelegramInlineKeyboard() {}

TelegramInlineButton *TelegramInlineKeyboard::createButton(TelegramCallback callback)
{
    auto size = _buttons.size();

    auto button = new TelegramInlineButton(callback, QString::number(size));
    _buttons.append(button);
    return button;
}

QString TelegramInlineKeyboard::json() const
{
    QString result = "reply_markup={\"inline_keyboard\":[";

    for (const auto button : _buttons) result += "[" + button->json() + "],";
    result.remove( result.size()-1, 1 );
    result += "]}";

    return result;
}

void TelegramInlineKeyboard::checkReply(const QJsonObject &callback_query)
{
    auto value = callback_query["data"];

    if ( value.isUndefined() )
    {
        qDebug() << "Callback data undefined!";
        return;
    }

    int callBackData = value.toInt();
    _buttons.at(callBackData)->doCallback();
}

TelegramInlineKeyboard::~TelegramInlineKeyboard()
{
    for (auto button : _buttons) delete button;
}


//TelegramReplyButton::TelegramReplyButton(QString text)
//    : _text(text)
//{}

//QString TelegramReplyButton::json() const
//{
//    return "{\"text\":\"" + _text + "\",\"request_poll\":{\"type\":\"regular\"}}"; //fix optional data
//}
