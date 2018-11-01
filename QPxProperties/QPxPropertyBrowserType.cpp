#include "QPxProperties/QPxPropertyBrowserType.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"
#include "QPxProperties/QPxPropertyBrowserEditor.h"
#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <QtCore/QMap>

#include <QtGui/QPainter>

namespace
{

class EnumCache
{
public:
    explicit EnumCache(const QStringList &values);
    explicit EnumCache(const QList<QPair<int, QString> > &values);

    QMap<int, QString> map;
};

EnumCache::EnumCache(const QStringList &values)
{
    for(int i = 0; i < values.count(); ++i)
    {
        map[i] = values[i];
    }
}

EnumCache::EnumCache(const QList<QPair<int, QString> > &values)
{
    for(int i = 0; i < values.count(); ++i)
    {
        map[values[i].first] = values[i].second;
    }
}

class PointCache
{
public:
    PointCache(QObject *parent) : type(new QPx::IntPropertyBrowserType(parent)) { }

    QPx::IntPropertyBrowserType *type;
};

}

QPx::PropertyBrowserType::PropertyBrowserType(QObject *parent) : QObject(parent)
{
}

void QPx::PropertyBrowserType::addProperties(PropertyBrowserItem *item, PropertyBrowserModel *model, const QModelIndex &parent) const
{
}

void QPx::PropertyBrowserType::updateProperties(PropertyBrowserItem *item, const QVariant &value) const
{
}

QString QPx::PropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return item->value().toString();
}

bool QPx::PropertyBrowserType::readOnly() const
{
    return false;
}

void QPx::PropertyBrowserType::paint(const PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const
{
    painter->drawText(rect.adjusted(2, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, valueText(item));
}

QPx::PropertyBrowserEditor *QPx::PropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::PropertyBrowserDialog *QPx::PropertyBrowserType::createDialog(const PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::StringPropertyBrowserType::StringPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QPx::PropertyBrowserEditor *QPx::StringPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new StringPropertyBrowserEditor(parent);
}

QPx::IntPropertyBrowserType::IntPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QPx::PropertyBrowserEditor *QPx::IntPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new IntPropertyBrowserEditor(parent);
}

QPx::FloatPropertyBrowserType::FloatPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QString QPx::FloatPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return QString::number(item->value().toFloat());
}

QPx::PropertyBrowserEditor *QPx::FloatPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new FloatPropertyBrowserEditor(parent);
}

QPx::BoolPropertyBrowserType::BoolPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QString QPx::BoolPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return QString();
}

QPx::ColorPropertyBrowserType::ColorPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

void QPx::ColorPropertyBrowserType::paint(const PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const
{
    painter->fillRect(rect.adjusted(2, 2, -4, -2), qvariant_cast<QColor>(item->value()));
}

QPx::PropertyBrowserDialog *QPx::ColorPropertyBrowserType::createDialog(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new ColorPropertyBrowserDialog(parent);
}

QPx::AbstractEnumPropertyBrowserType::AbstractEnumPropertyBrowserType(const QStringList &values, QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<EnumCache>(values);
}

QPx::AbstractEnumPropertyBrowserType::AbstractEnumPropertyBrowserType(const QList<QPair<int, QString> > &values, QObject *parent)
{
    cache.alloc<EnumCache>(values);
}

QString QPx::AbstractEnumPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return cache.get<EnumCache>().map[item->value().toInt()];
}

QPx::PropertyBrowserEditor *QPx::AbstractEnumPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new EnumPropertyBrowserEditor(this, cache.get<EnumCache>().map, parent);
}

QPx::PointPropertyBrowserType::PointPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<PointCache>(this);
}

void QPx::PointPropertyBrowserType::addProperties(QPx::PropertyBrowserItem *item, QPx::PropertyBrowserModel *model, const QModelIndex &parent) const
{
    auto type = cache.get<PointCache>().type;

    item->addProperty(new PropertyBrowserItem(type, model, parent, "X", item->flags(), item->value().toPoint().x(), item));
    connect(item->property(0), SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));

    item->addProperty(new PropertyBrowserItem(type, model, parent, "Y", item->flags(), item->value().toPoint().y(), item));
    connect(item->property(1), SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
}

void QPx::PointPropertyBrowserType::updateProperties(QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    item->property(0)->setValue(value.toPoint().x());
    item->property(1)->setValue(value.toPoint().y());
}

QString QPx::PointPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    auto p = item->value().toPoint();
    return QString("%1, %2").arg(p.x()).arg(p.y());
}

bool QPx::PointPropertyBrowserType::readOnly() const
{
    return true;
}

void QPx::PointPropertyBrowserType::changed(const QVariant &value)
{
    auto item = static_cast<PropertyBrowserItem*>(sender());
    auto parent = static_cast<PropertyBrowserItem*>(item->parent());

    auto p = parent->value().toPoint();

    if(item->name() == "X")
    {
        p.setX(value.toInt());
    }
    else
    {
        p.setY(value.toInt());
    }

    parent->setValue(p);
}

