#ifndef INTERNET_H
#define INTERNET_H

#include <QString>

class Internet
{
public:
    virtual void loop() = 0;

    virtual void get(QString url) = 0;
    virtual bool replyReady() const = 0;
    virtual QString reply() const = 0;

    virtual ~Internet() {}
};


#include <QNetworkAccessManager>
#include <QNetworkReply>

class InternetPc : public Internet
{
public:
    InternetPc();

    void loop() override;

    void get(QString url) override;
    bool replyReady() const override;
    QString reply() const override;

private:
    QString _replyStr = "";

    QNetworkAccessManager _internet;
    QNetworkReply *_replyObj = nullptr;
    bool _replyReady = false;
};

#endif // INTERNET_H
