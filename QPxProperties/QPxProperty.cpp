#include "QPxProperties/QPxProperty.h"

namespace
{

class Cache
{
public:
    Cache(const QString &name, QPx::Property::Flags flags, const QVariant &value) : name(name), flags(flags), value(value) { }

    QString name;
    QPx::Property::Flags flags;
    QVariant value;
};

}

QPx::Property::Property()
{
    cache.alloc<Cache>(QString(), Flags(), QVariant());
}

QPx::Property::Property(const QString &name, Flags flags, const QVariant &value)
{
    cache.alloc<Cache>(name, flags, value);
}

void QPx::Property::setName(const QString &value)
{
    cache.get<Cache>().name = value;
}

void QPx::Property::setFlags(const Flags &value)
{
    cache.get<Cache>().flags = value;
}

void QPx::Property::setVariant(const QVariant &value)
{
    cache.get<Cache>().value = value;
}

void QPx::Property::setValue(const char *value)
{
    cache.get<Cache>().value = QString(value);
}

QString QPx::Property::name() const
{
    return cache.get<Cache>().name;
}

QPx::Property::Flags QPx::Property::flags() const
{
    return cache.get<Cache>().flags;
}

QVariant QPx::Property::variant() const
{
    return cache.get<Cache>().value;
}
