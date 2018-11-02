#include "QPxProperties/QPxPropertyBrowserType.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"
#include "QPxProperties/QPxPropertyBrowserEditor.h"
#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <QtCore/QMap>

#include <QtGui/QPainter>

#include <QtWidgets/QStyleOptionViewItem>

namespace
{

class NumericCache
{
public:
    NumericCache(const QVariant &min, const QVariant &max) : min(min), max(max) { }

    template<typename T> bool validate(const QVariant &value) const;

    QVariant min;
    QVariant max;
};

template<typename T> bool NumericCache::validate(const QVariant &value) const
{
    if(min.isValid() && qvariant_cast<T>(value) < qvariant_cast<T>(min)) return false;
    if(max.isValid() && qvariant_cast<T>(value) > qvariant_cast<T>(max)) return false;

    return true;
}

class EnumCache
{
public:
    explicit EnumCache(const QStringList &values){ for(int i = 0; i < values.count(); ++i) map[i] = values[i]; }
    explicit EnumCache(const QList<QPair<int, QString> > &values){ for(int i = 0; i < values.count(); ++i) map[values[i].first] = values[i].second; }

    QMap<int, QString> map;
};

class FlagCache
{
public:
    FlagCache(const QStringList &values, QObject *parent) : type(new QPx::BoolPropertyBrowserType(parent)) { unsigned v = 1; foreach(const QString &value, values){ map[v] = value; v *= 2; } }

    QPx::BoolPropertyBrowserType *type;
    QMap<unsigned, QString> map;
};

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

bool QPx::PropertyBrowserType::checkable() const
{
    return false;
}

bool QPx::PropertyBrowserType::validate(const QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    return true;
}

void QPx::PropertyBrowserType::paint(const PropertyBrowserItem *item, QPainter *painter, const QStyleOptionViewItem &option) const
{
    auto r = option.rect.adjusted(2, 0, 0, 0);

    auto pen = painter->pen();
    if(!(option.state & QStyle::State_Enabled))
    {
        painter->setPen(option.palette.color(QPalette::Disabled, QPalette::Text));
    }

    painter->drawText(r, Qt::AlignVCenter | Qt::AlignLeft, QFontMetrics(painter->font()).elidedText(valueText(item), Qt::ElideRight, r.width()));

    painter->setPen(pen);
}

QPx::PropertyBrowserEditor *QPx::PropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::PropertyBrowserDialog *QPx::PropertyBrowserType::createDialog(const PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::GroupPropertyBrowserType::GroupPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

bool QPx::GroupPropertyBrowserType::readOnly() const
{
    return true;
}

QPx::StringPropertyBrowserType::StringPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QPx::PropertyBrowserEditor *QPx::StringPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new StringPropertyBrowserEditor(parent);
}

QPx::NumericPropertyBrowserType::NumericPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<NumericCache>(QVariant(), QVariant());
}

QPx::NumericPropertyBrowserType::NumericPropertyBrowserType(const QVariant &min, const QVariant &max, QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<NumericCache>(min, max);
}

bool QPx::IntPropertyBrowserType::validate(const QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    return cache.get<NumericCache>().validate<int>(value);
}

QPx::PropertyBrowserEditor *QPx::IntPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    auto &c = cache.get<NumericCache>();
    return new IntPropertyBrowserEditor(c.min, c.max, parent);
}

QString QPx::FloatPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return QString::number(item->value().toFloat());
}

bool QPx::FloatPropertyBrowserType::validate(const QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    return cache.get<NumericCache>().validate<float>(value);
}

QPx::PropertyBrowserEditor *QPx::FloatPropertyBrowserType::createEditor(const PropertyBrowserItem *item, QWidget *parent) const
{
    auto &c = cache.get<NumericCache>();
    return new FloatPropertyBrowserEditor(c.min, c.max, parent);
}

QPx::BoolPropertyBrowserType::BoolPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QString QPx::BoolPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    return QString();
}

bool QPx::BoolPropertyBrowserType::checkable() const
{
    return true;
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

QPx::AbstractFlagPropertyBrowserType::AbstractFlagPropertyBrowserType(const QStringList &values, QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<FlagCache>(values, this);
}

void QPx::AbstractFlagPropertyBrowserType::addProperties(QPx::PropertyBrowserItem *item, QPx::PropertyBrowserModel *model, const QModelIndex &parent) const
{
    auto &c = cache.get<FlagCache>();

    foreach(auto v, c.map.keys())
    {
        auto i = item->addProperty(new QPx::PropertyBrowserItem(c.type, model, parent, c.map[v], item->flags(), toUnsigned(item->value()) & v, item));
        connect(i, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
    }
}

void QPx::AbstractFlagPropertyBrowserType::updateProperties(QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    auto val = toUnsigned(value);

    unsigned v = 1;
    for(int i = 0; i < item->propertyCount(); ++i)
    {
        item->property(i)->setValue(val & v);
        v *= 2;
    }
}

QString QPx::AbstractFlagPropertyBrowserType::valueText(const QPx::PropertyBrowserItem *item) const
{
    auto &map = cache.get<FlagCache>().map;
    auto value = toUnsigned(item->value());

    QStringList vs;
    foreach(auto v, map.keys())
    {
        if(value & v) vs.append(map[v]);
    }

    return vs.join(", ");
}

bool QPx::AbstractFlagPropertyBrowserType::readOnly() const
{
    return true;
}

void QPx::AbstractFlagPropertyBrowserType::changed(const QVariant &value)
{
    auto &map = cache.get<FlagCache>().map;

    auto item = static_cast<PropertyBrowserItem*>(sender());
    auto parent = static_cast<PropertyBrowserItem*>(item->parent());

    auto p = toUnsigned(parent->value());

    foreach(auto v, map.keys())
    {
        if(item->name() == map[v])
        {
            value.toBool() ? p |= v : p &= (~v);
            break;
        }
    }

    parent->setValue(toFlagValue(p));
}

QPx::ColorPropertyBrowserType::ColorPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

void QPx::ColorPropertyBrowserType::paint(const PropertyBrowserItem *item, QPainter *painter, const QStyleOptionViewItem &option) const
{
    painter->fillRect(option.rect.adjusted(2, 2, -4, -2), qvariant_cast<QColor>(item->value()));
}

QPx::PropertyBrowserDialog *QPx::ColorPropertyBrowserType::createDialog(const PropertyBrowserItem *item, QWidget *parent) const
{
    return new ColorPropertyBrowserDialog(parent);
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
