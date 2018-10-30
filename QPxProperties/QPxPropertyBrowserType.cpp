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

}

QPx::PropertyBrowserType::PropertyBrowserType(QObject *parent) : QObject(parent)
{
}

QString QPx::PropertyBrowserType::valueText(const QPx::PropertyBrowserItem *item) const
{
    return item->value().toString();
}

void QPx::PropertyBrowserType::paint(const QPx::PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const
{
    painter->drawText(rect.adjusted(2, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, valueText(item));
}

QPx::PropertyBrowserEditor *QPx::PropertyBrowserType::createEditor(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::PropertyBrowserDialog *QPx::PropertyBrowserType::createDialog(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return nullptr;
}

QPx::StringPropertyBrowserType::StringPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QPx::PropertyBrowserEditor *QPx::StringPropertyBrowserType::createEditor(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return new StringPropertyBrowserEditor(parent);
}

QPx::IntPropertyBrowserType::IntPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QPx::PropertyBrowserEditor *QPx::IntPropertyBrowserType::createEditor(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return new IntPropertyBrowserEditor(parent);
}

QPx::FloatPropertyBrowserType::FloatPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QString QPx::FloatPropertyBrowserType::valueText(const QPx::PropertyBrowserItem *item) const
{
    return QString::number(item->value().toFloat());
}

QPx::PropertyBrowserEditor *QPx::FloatPropertyBrowserType::createEditor(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return new FloatPropertyBrowserEditor(parent);
}

QPx::BoolPropertyBrowserType::BoolPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

QString QPx::BoolPropertyBrowserType::valueText(const QPx::PropertyBrowserItem *item) const
{
    return QString();
}

QPx::ColorPropertyBrowserType::ColorPropertyBrowserType(QObject *parent) : PropertyBrowserType(parent)
{
}

void QPx::ColorPropertyBrowserType::paint(const QPx::PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const
{
    painter->fillRect(rect.adjusted(2, 2, -4, -2), qvariant_cast<QColor>(item->value()));
}

QPx::PropertyBrowserDialog *QPx::ColorPropertyBrowserType::createDialog(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return new ColorPropertyBrowserDialog(parent);
}

QPx::EnumPropertyBrowserType::EnumPropertyBrowserType(const QStringList &values, QObject *parent) : PropertyBrowserType(parent)
{
    cache.alloc<EnumCache>(values);
}

QPx::EnumPropertyBrowserType::EnumPropertyBrowserType(const QList<QPair<int, QString> > &values, QObject *parent)
{
    cache.alloc<EnumCache>(values);
}

QString QPx::EnumPropertyBrowserType::valueText(const QPx::PropertyBrowserItem *item) const
{
    return cache.get<EnumCache>().map[item->value().toInt()];
}

QPx::PropertyBrowserEditor *QPx::EnumPropertyBrowserType::createEditor(const QPx::PropertyBrowserItem *item, QWidget *parent) const
{
    return new EnumPropertyBrowserEditor(cache.get<EnumCache>().map, parent);
}
