#ifndef IVIEW_H
#define IVIEW_H

#include <QString>

class IView
{
public:
    virtual void modelUpdated() = 0;
    virtual void alarm(QString message) = 0;

    virtual ~IView() {}
};

#endif // IVIEW_H
