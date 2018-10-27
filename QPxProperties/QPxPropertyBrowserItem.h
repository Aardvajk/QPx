#ifndef QPX_PROPERTYBROWSERITEM_H
#define QPX_PROPERTYBROWSERITEM_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QPoint>

#include <pcx/aligned_store.h>

class QModelIndex;

namespace QPx
{

class PropertyBrowserModel;

class PropertyBrowserItem : public QObject
{
    Q_OBJECT

public:
    PropertyBrowserItem(const QString &name, const QVariant &value, QObject *parent = nullptr);

    QString name() const;
    QVariant value() const;

    virtual QString valueText() const;

private:
    pcx::aligned_store<24> cache;
};

class IntPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    IntPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);
};

class PointPropertyBrowserItem : public PropertyBrowserItem
{
    Q_OBJECT

public:
    PointPropertyBrowserItem(PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    virtual QString valueText() const;
};

}

Q_DECLARE_METATYPE(QPx::PropertyBrowserItem*)

#endif // QPX_PROPERTYBROWSERITEM_H
