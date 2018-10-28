#include "QPxProperties/QPxPropertyBrowserItem.h"

#include "QPxProperties/QPxPropertyBrowserModel.h"

#include <QtWidgets/QSpinBox>

namespace
{

class Cache
{
public:
    Cache(const QString &name, const QVariant &value) : name(name), value(value) { }

    QString name;
    QVariant value;
};

}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QString &name, const QVariant &value, QObject *parent) : QObject(parent)
{
    cache.alloc<Cache>(name, value);
}

QString QPx::PropertyBrowserItem::name() const
{
    return cache.get<Cache>().name;
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<Cache>().value;
}

QString QPx::PropertyBrowserItem::valueText() const
{
    return value().toString();
}

void QPx::PropertyBrowserItem::setValue(const QVariant &value)
{
    cache.get<Cache>().value = value;
    emit valueChanged(value);
}

QPx::StringPropertyBrowserItem::StringPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

QPx::IntPropertyBrowserItem::IntPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

QPx::PointPropertyBrowserItem::PointPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    auto mi = model->appendRow(this, index);

    auto p = value.toPoint();

    x = new IntPropertyBrowserItem(model, mi, "X", p.x(), this);
    connect(x, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));

    y = new IntPropertyBrowserItem(model, mi, "Y", p.y(), this);
    connect(y, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
}

QString QPx::PointPropertyBrowserItem::valueText() const
{
    auto p = value().toPoint();
    return QString("%1, %2").arg(p.x()).arg(p.y());
}

void QPx::PointPropertyBrowserItem::changed(const QVariant &value)
{
    QPoint p = this->value().toPoint();

    if(sender() == x)
    {
        p.setX(value.toInt());
    }
    else
    {
        p.setY(value.toInt());
    }

    setValue(p);
}
