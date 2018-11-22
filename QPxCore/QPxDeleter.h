#ifndef QPX_DELETER_H
#define QPX_DELETER_H

#include <QtCore/QObject>

namespace QPx
{

class AbstractDeleter : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDeleter(QObject *parent = nullptr);

    virtual void operator()(void *ptr) const = 0;
};

template<typename T> class Deleter : public AbstractDeleter
{
public:
    Deleter(QObject *parent = nullptr) : AbstractDeleter(parent) { }

    virtual void operator()(void *ptr) const override { delete static_cast<T*>(ptr); }
};

}

#endif // QPX_DELETER_H
