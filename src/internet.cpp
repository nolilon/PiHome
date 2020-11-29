#include "internet.h"

#include "bottoken.h"
#include <QCoreApplication>

InternetPc::InternetPc()
{}

void InternetPc::loop()
{
    QCoreApplication::processEvents();
    if ( _replyObj != nullptr && _replyObj->isFinished() )
    {
        QTextStream str(_replyObj);
        _replyStr = str.readAll().toStdString().c_str();
        _replyReady = true;

//        if (_replyStr.length() > 0)
//            qDebug() << _replyStr;
    }
}

void InternetPc::get(QString url)
{
    _replyStr.clear();
    if ( _replyObj != nullptr ) _replyObj->abort();

    auto encodedUrl = QUrl( url );
    _replyObj = _internet.get(QNetworkRequest(encodedUrl));
    _replyReady = false;
//    qDebug() << "Get request sent:\n" << url;
}

bool InternetPc::replyReady() const
{
    return _replyReady;
}

QString InternetPc::reply() const
{
    return _replyStr;
}
