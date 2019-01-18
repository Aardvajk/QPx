#ifndef QPX_PROPERTYBROWSERITEM_H
#define QPX_PROPERTYBROWSERITEM_H

#include <QtCore/QObject>

#include <pcx/flags.h>
#include <pcx/aligned_store.h>

class QModelIndex;

namespace QPx
{

class PropertyBrowserItemProxy;
class PropertyBrowserModel;
class PropertyBrowserType;

class PropertyBrowserItem : public QObject
{
    Q_OBJECT

public:
    enum class Flag
    {
        ReadOnly = 1
    };

    using Flags = pcx::flags<Flag>;

    PropertyBrowserItem(const PropertyBrowserType *type, PropertyBrowserModel *model, const QModelIndex &index, const QString &name, Flags flags, const QVariant &value, QObject *parent = nullptr);

    const PropertyBrowserType *type() const;

    Flags flags() const;
    QString name() const;
    QVariant value() const;

    PropertyBrowserItem *addItem(PropertyBrowserItem *item);
    QVector<PropertyBrowserItem*> items() const;

signals:
    void valueChanged(const QVariant &value);

public slots:
    void setValue(const QVariant &value);

private:
    pcx::aligned_store<56> cache;
};

}

template<> struct pcx_is_flag_enum<QPx::PropertyBrowserItem::Flag> : std::true_type { };

#endif // QPX_PROPERTYBROWSERITEM_H
