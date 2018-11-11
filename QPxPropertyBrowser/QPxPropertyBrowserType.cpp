#include "QPxPropertyBrowser/QPxPropertyBrowserType.h"

#include "QPxPropertyBrowser/QPxPropertyBrowserItem.h"
#include "QPxPropertyBrowser/QPxPropertyBrowserEditor.h"
#include "QPxPropertyBrowser/QPxPropertyBrowserDialog.h"

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
    if(value.toString().isEmpty()) return false;

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

QPair<QVariant, QVariant> pointValues(const QVariant &value)
{
    return value.isValid() ? qMakePair(QVariant(value.toPoint().x()), QVariant(value.toPoint().y())) : qMakePair(QVariant(), QVariant());
}

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

bool QPx::PropertyBrowserType::validate(const QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    return true;
}

bool QPx::PropertyBrowserType::compare(const QVariant &value1, const QVariant &value2) const
{
    return value1 == value2;
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
    return item->value().isValid() ? QString::number(item->value().toFloat()) : QString();
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
    return item->value().isValid() ? cache.get<EnumCache>().map[item->value().toInt()] : QString();
}

bool QPx::AbstractEnumPropertyBrowserType::compare(const QVariant &value1, const QVariant &value2) const
{
    return value1.toInt() == value2.toInt();
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
        auto i = item->addItem(new QPx::PropertyBrowserItem(c.type, model, parent, c.map[v], item->flags(), toUnsigned(item->value()) & v, item));
        connect(i, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
    }
}

void QPx::AbstractFlagPropertyBrowserType::updateProperties(QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    auto val = toUnsigned(value);

    unsigned v = 1;
    for(int i = 0; i < item->items().count(); ++i)
    {
        item->items()[i]->setValue(val & v);
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

bool QPx::AbstractFlagPropertyBrowserType::compare(const QVariant &value1, const QVariant &value2) const
{
    return toUnsigned(value1) == toUnsigned(value2);
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

bool QPx::ColorPropertyBrowserType::compare(const QVariant &value1, const QVariant &value2) const
{
    return qvariant_cast<QColor>(value1) == qvariant_cast<QColor>(value2);
}

void QPx::ColorPropertyBrowserType::paint(const PropertyBrowserItem *item, QPainter *painter, const QStyleOptionViewItem &option) const
{
    auto r = option.rect.adjusted(2, 2, -2, -2);
    auto c = qvariant_cast<QColor>(item->value());

    if(!item->value().isValid() || c.alpha() < 255)
    {
        painter->fillRect(r, QBrush(QColor(220, 220, 220), Qt::DiagCrossPattern));
    }

    if(item->value().isValid())
    {
        painter->fillRect(r, c);
    }
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

    auto values = pointValues(item->value());

    item->addItem(new PropertyBrowserItem(type, model, parent, "X", item->flags(), values.first, item));
    connect(item->items()[0], SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));

    item->addItem(new PropertyBrowserItem(type, model, parent, "Y", item->flags(), values.second, item));
    connect(item->items()[1], SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
}

void QPx::PointPropertyBrowserType::updateProperties(QPx::PropertyBrowserItem *item, const QVariant &value) const
{
    auto values = pointValues(value);

    item->items()[0]->setValue(values.first);
    item->items()[1]->setValue(values.second);
}

QString QPx::PointPropertyBrowserType::valueText(const PropertyBrowserItem *item) const
{
    if(item->value().isValid())
    {
        auto p = item->value().toPoint();
        return QString("%1, %2").arg(p.x()).arg(p.y());
    }

    return QString();
}

bool QPx::PointPropertyBrowserType::readOnly() const
{
    return true;
}

bool QPx::PointPropertyBrowserType::compare(const QVariant &value1, const QVariant &value2) const
{
    return value1.toPoint() == value2.toPoint();
}

void QPx::PointPropertyBrowserType::changed(const QVariant &value)
{
    auto item = static_cast<PropertyBrowserItem*>(sender());
    auto parent = static_cast<PropertyBrowserItem*>(item->parent());

    auto p = parent->value().toPoint();

    switch(parent->items().indexOf(item))
    {
        case 0: p.setX(value.toInt()); break;
        case 1: p.setY(value.toInt()); break;
    }

    parent->setValue(p);
}
