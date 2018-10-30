#include "QPxProperties/QPxPropertyBrowserItem.h"

#include "QPxProperties/QPxPropertyBrowserModel.h"
#include "QPxProperties/QPxPropertyBrowserEditor.h"
#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <pcx/scoped_lock.h>

#include <QtGui/QPainter>

#include <QtWidgets/QSpinBox>

namespace
{

class ItemCache
{
public:
    ItemCache(const QString &name, const QVariant &value) : name(name), value(value) { }

    QString name;
    QVariant value;
};

class PointCache
{
public:
    PointCache() : lock(false) { }

    bool lock;

    QPx::PropertyBrowserItem *x;
    QPx::PropertyBrowserItem *y;
};

}

QPx::PropertyBrowserItem::PropertyBrowserItem(const QString &name, const QVariant &value, QObject *parent) : QObject(parent)
{
    cache.alloc<ItemCache>(name, value);
}

QString QPx::PropertyBrowserItem::name() const
{
    return cache.get<ItemCache>().name;
}

QVariant QPx::PropertyBrowserItem::value() const
{
    return cache.get<ItemCache>().value;
}

QString QPx::PropertyBrowserItem::valueText() const
{
    return value().toString();
}

void QPx::PropertyBrowserItem::paint(QPainter *painter, const QRect &rect) const
{
    painter->drawText(rect.adjusted(2, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, valueText());
}

QPx::PropertyBrowserEditor *QPx::PropertyBrowserItem::createEditor(QWidget *parent) const
{
    return nullptr;
}

QPx::PropertyBrowserDialog *QPx::PropertyBrowserItem::createDialog(QWidget *parent) const
{
    return nullptr;
}

void QPx::PropertyBrowserItem::setValue(const QVariant &value)
{
    cache.get<ItemCache>().value = value;
    emit valueChanged(value);
}

QPx::StringPropertyBrowserItem::StringPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

QPx::PropertyBrowserEditor *QPx::StringPropertyBrowserItem::createEditor(QWidget *parent) const
{
    return new StringPropertyBrowserEditor(parent);
}

QPx::IntPropertyBrowserItem::IntPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

QPx::PropertyBrowserEditor *QPx::IntPropertyBrowserItem::createEditor(QWidget *parent) const
{
    return new IntPropertyBrowserEditor(parent);
}

QPx::BoolPropertyBrowserItem::BoolPropertyBrowserItem(QPx::PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

QString QPx::BoolPropertyBrowserItem::valueText() const
{
    return QString();
}

QPx::PointPropertyBrowserItem::PointPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    auto &c = cache.alloc<PointCache>();

    auto i = model->appendRow(this, index);
    auto p = value.toPoint();

    c.x = new IntPropertyBrowserItem(model, i, "X", p.x(), this);
    connect(c.x, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));

    c.y = new IntPropertyBrowserItem(model, i, "Y", p.y(), this);
    connect(c.y, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
}

QString QPx::PointPropertyBrowserItem::valueText() const
{
    auto p = value().toPoint();
    return QString("%1, %2").arg(p.x()).arg(p.y());
}

void QPx::PointPropertyBrowserItem::setValue(const QVariant &value)
{
    auto &c = cache.get<PointCache>();

    if(!c.lock)
    {
        auto g = pcx::scoped_lock(c.lock);

        c.x->setValue(value.toPoint().x());
        c.y->setValue(value.toPoint().y());

        PropertyBrowserItem::setValue(value);
    }
}

void QPx::PointPropertyBrowserItem::changed(const QVariant &value)
{
    auto &c = cache.get<PointCache>();
    QPoint p = this->value().toPoint();

    if(sender() == c.x)
    {
        p.setX(value.toInt());
    }
    else
    {
        p.setY(value.toInt());
    }

    setValue(p);
}

QPx::ColorPropertyBrowserItem::ColorPropertyBrowserItem(QPx::PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent) : PropertyBrowserItem(name, value, parent)
{
    model->appendRow(this, index);
}

void QPx::ColorPropertyBrowserItem::paint(QPainter *painter, const QRect &rect) const
{
    painter->fillRect(rect.adjusted(2, 2, -4, -2), qvariant_cast<QColor>(value()));
}

QPx::PropertyBrowserDialog *QPx::ColorPropertyBrowserItem::createDialog(QWidget *parent) const
{
    return new ColorPropertyBrowserDialog(parent);
}
