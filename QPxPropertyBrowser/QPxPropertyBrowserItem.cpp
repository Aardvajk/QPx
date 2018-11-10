#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

#include "QPxPropertyBrowser/QPxPropertyBrowserModel.h"
#include "QPxPropertyBrowser/QPxPropertyBrowserType.h"

#include <pcx/scoped_ptr.h>
#include <pcx/scoped_lock.h>

#include <QtGui/QPainter>

namespace
{

class Cache
{
public:
    Cache(const QPx::PropertyBrowserType *type, const QString &name, QPx::PropertyBrowserItem::Flags flags, const QVariant &value) : type(type), name(name), flags(flags), value(value), lock(false) { }

    const QPx::PropertyBrowserType *type;
    QString name;
    QPx::PropertyBrowserItem::Flags flags;
    QVariant value;
    QList<QPx::PropertyBrowserItem*> props;
    bool lock;
};

}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QPx::PropertyBrowserType *type, QPx::PropertyBrowserModel *model, const QModelIndex &index, const QString &name, Flags flags, const QVariant &value, QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>(type, name, flags, value);
    auto m = model->appendRow(this, index);

    type->addProperties(this, model, m);
}

const QPx::PropertyBrowserType *QPx::PropertyBrowserItem::type() const
{
    return cache.get<Cache>().type;
}

QPx::PropertyBrowserItem::Flags QPx::PropertyBrowserItem::flags() const
{
    return cache.get<Cache>().flags;
}

QString QPx::PropertyBrowserItem::name() const
{
    return cache.get<Cache>().name;
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<Cache>().value;
}

QPx::PropertyBrowserItem *QPx::PropertyBrowserItem::addItem(PropertyBrowserItem *item)
{
    cache.get<Cache>().props.append(item);
    return item;
}

QList<QPx::PropertyBrowserItem*> QPx::PropertyBrowserItem::items() const
{
    return cache.get<Cache>().props;
}

void QPx::PropertyBrowserItem::setValue(const QVariant &value)
{
    if(this->value() != value)
    {
        auto &c = cache.get<Cache>();
        if(!c.lock)
        {
            auto g = pcx::scoped_lock(c.lock);

            type()->updateProperties(this, value);

            c.value = value;
            emit valueChanged(value);
        }
    }
}
