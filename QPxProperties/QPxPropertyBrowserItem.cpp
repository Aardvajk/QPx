#include "QPxProperties/QPxPropertyBrowserItem.h"

#include "QPxProperties/QPxPropertyBrowserModel.h"

#include <pcx/scoped_lock.h>

#include <QtGui/QPainter>

#include <QtWidgets/QSpinBox>

namespace
{

class ItemCache
{
public:
    ItemCache(const QPx::PropertyBrowserType *type, const QString &name, const QVariant &value) : type(type), name(name), value(value) { }

    const QPx::PropertyBrowserType *type;
    QString name;
    QVariant value;
};

}

QPx::PropertyBrowserItem::PropertyBrowserItem(const PropertyBrowserType *type, const QString &name, const QVariant &value, QObject *parent) : QObject(parent)
{
    cache.alloc<ItemCache>(type, name, value);
}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QPx::PropertyBrowserType *type, QPx::PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent)
{
    cache.alloc<ItemCache>(type, name, value);
    model->appendRow(this, index);
}

const QPx::PropertyBrowserType *QPx::PropertyBrowserItem::type() const
{
    return cache.get<ItemCache>().type;
}

QString QPx::PropertyBrowserItem::name() const
{
    return cache.get<ItemCache>().name;
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<ItemCache>().value;
}

void QPx::PropertyBrowserItem::setValue(const QVariant &value)
{
    cache.get<ItemCache>().value = value;
    emit valueChanged(value);
}
