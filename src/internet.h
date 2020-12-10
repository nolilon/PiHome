#ifndef INTERNET_H
#define INTERNET_H

#include <QString>
#include <functional>

typedef std::function<void()> Callback;

class Internet
{
public:
    virtual void get(QString url) = 0;

    virtual void setOnReply(Callback callback) = 0;
    virtual QString reply() const = 0;

    virtual ~Internet() {}
};


#include <QNetworkAccessManager>
#include <QNetworkReply>

class InternetPc : public Internet
{
public:
    InternetPc();

    void get(QString url) override;
    void setOnReply(Callback callback) override;
    QString reply() const override;

private:
    void requestDone();
    QString _replyStr = "";
    Callback _onReply = nullptr;

    QNetworkAccessManager _internet;
    QNetworkReply *_replyObj = nullptr;
};

#endif // INTERNET_H
