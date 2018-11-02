#include "QPxProperties/QPxPropertyBrowserItem.h"

#include "QPxProperties/QPxPropertyBrowserModel.h"
#include "QPxProperties/QPxPropertyBrowserType.h"

#include <pcx/scoped_lock.h>

#include <QtGui/QPainter>

#include <QtWidgets/QSpinBox>

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
    QVector<QPx::PropertyBrowserItem*> props;
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

QString QPx::PropertyBrowserItem::name() const
{
    return cache.get<Cache>().name;
}

QPx::PropertyBrowserItem::Flags QPx::PropertyBrowserItem::flags() const
{
    return cache.get<Cache>().flags;
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<Cache>().value;
}

QPx::PropertyBrowserItem *QPx::PropertyBrowserItem::addItem(PropertyBrowserItem *property)
{
    cache.get<Cache>().props.append(property);
    return property;
}

QPx::PropertyBrowserItem *QPx::PropertyBrowserItem::item(int index) const
{
    return cache.get<Cache>().props[index];
}

int QPx::PropertyBrowserItem::itemCount() const
{
    return cache.get<Cache>().props.count();
}

int QPx::PropertyBrowserItem::itemIndex(const QPx::PropertyBrowserItem *item) const
{
    auto &p = cache.get<Cache>().props;
    for(int i = 0; i < p.count(); ++i)
    {
        if(p[i] == item) return i;
    }

    return -1;
}

void QPx::PropertyBrowserItem::setValue(const QVariant &value)
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
