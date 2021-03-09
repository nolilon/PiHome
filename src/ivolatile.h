#ifndef IVOLATILE_H
#define IVOLATILE_H

#include <functional>
typedef std::function<void()> OnUpdated;

class IVolatile
{
public:
    virtual void setOnUpdated(OnUpdated onUpdated) = 0;
    virtual ~IVolatile() {};
};

#endif // IVOLATILE_H
