#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"

#include "QPxPropertyBrowser/QPxPropertyBrowserModel.h"
#include "QPxPropertyBrowser/QPxPropertyBrowserType.h"

#include <pcx/scoped_ptr.h>
#include <pcx/scoped_lock.h>

#include <QtGui/QPainter>

namespace
{

class Proxy : public QPx::PropertyBrowserItemProxy
{
public:
    Proxy(const QString &name, const QVariant &value) : n(name), v(value) { }

    virtual QString name() const override { return n; }
    virtual QVariant value() const override { return v; }

    virtual void setValue(const QVariant &value) override { v = value; }

private:
    QString n;
    QVariant v;
};

class Cache
{
public:
    Cache(const QPx::PropertyBrowserType *type, QPx::PropertyBrowserItem::Flags flags, QPx::PropertyBrowserItemProxy *proxy) : type(type), flags(flags), proxy(proxy), lock(false) { }

    const QPx::PropertyBrowserType *type;
    QPx::PropertyBrowserItem::Flags flags;
    pcx::scoped_ptr<QPx::PropertyBrowserItemProxy> proxy;
    QList<QPx::PropertyBrowserItem*> props;
    bool lock;
};

}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QPx::PropertyBrowserType *type, QPx::PropertyBrowserModel *model, const QModelIndex &index, Flags flags, const QString &name, const QVariant &value, QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>(type, flags, new Proxy(name, value));
    auto m = model->appendRow(this, index);

    type->addProperties(this, model, m);
}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QPx::PropertyBrowserType *type, QPx::PropertyBrowserModel *model, const QModelIndex &index, Flags flags, QPx::PropertyBrowserItemProxy *proxy, QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>(type, flags, proxy);
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
    return cache.get<Cache>().proxy->name();
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<Cache>().proxy->value();
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
    auto &c = cache.get<Cache>();
    if(!c.lock)
    {
        auto g = pcx::scoped_lock(c.lock);

        type()->updateProperties(this, value);

        c.proxy->setValue(value);
        emit valueChanged(value);
    }
}
