#include "internet.h"


InternetPc::InternetPc()
{}

void InternetPc::get(QString url)
{
    _replyStr.clear();
    if ( _replyObj != nullptr )
    {
        delete _replyObj;
        _replyObj = nullptr;
    }

    auto encodedUrl = QUrl( url );
    _replyObj = _internet.get(QNetworkRequest(encodedUrl));
    QObject::connect( _replyObj, &QNetworkReply::finished, [this] () {this->requestDone();} );
//    qDebug() << "Get request sent:\n" << url;
}

void InternetPc::requestDone()
{
    _replyStr = _replyObj->readAll();
    _replyObj->deleteLater();
    _replyObj = nullptr;

    _onReply();

//    if (_replyStr.length() > 0)
//        qDebug() << _replyStr;
}

void InternetPc::setOnReply(Callback callback)
{
    _onReply = callback;
}

QString InternetPc::reply() const
{
    return _replyStr;
}
