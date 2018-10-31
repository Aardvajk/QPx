#ifndef QPX_PROPERTYBROWSERITEM_H
#define QPX_PROPERTYBROWSERITEM_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

class QModelIndex;

namespace QPx
{

class PropertyBrowserModel;
class PropertyBrowserType;

class PropertyBrowserItem : public QObject
{
    Q_OBJECT

public:
    PropertyBrowserItem(const PropertyBrowserType *type, PropertyBrowserModel *model, const QModelIndex &index, const QString &name, const QVariant &value, QObject *parent = nullptr);

    const PropertyBrowserType *type() const;

    QString name() const;
    QVariant value() const;

    PropertyBrowserItem *addProperty(PropertyBrowserItem *property);
    PropertyBrowserItem *property(int index) const;

    int propertyCount() const;

signals:
    void valueChanged(const QVariant &value);

public slots:
    void setValue(const QVariant &value);

private:
    pcx::aligned_store<48> cache;
};

}

#endif // QPX_PROPERTYBROWSERITEM_H
