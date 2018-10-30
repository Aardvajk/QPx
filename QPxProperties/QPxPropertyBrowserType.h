#ifndef QPXPROPERTYBROWSERTYPE_H
#define QPXPROPERTYBROWSERTYPE_H

#include <QtCore/QObject>

#include <pcx/aligned_store.h>

class QWidget;
class QPainter;

namespace QPx
{

class PropertyBrowserItem;
class PropertyBrowserEditor;
class PropertyBrowserDialog;

class PropertyBrowserType : public QObject
{
    Q_OBJECT

public:
    explicit PropertyBrowserType(QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const;

    virtual void paint(const PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const;
    virtual PropertyBrowserDialog *createDialog(const PropertyBrowserItem *item, QWidget *parent) const;
};

class StringPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit StringPropertyBrowserType(QObject *parent = nullptr);

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class IntPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit IntPropertyBrowserType(QObject *parent = nullptr);

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class FloatPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit FloatPropertyBrowserType(QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const override;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class BoolPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit BoolPropertyBrowserType(QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const override;
};

class ColorPropertyBrowserType : public PropertyBrowserType
{
    Q_OBJECT

public:
    explicit ColorPropertyBrowserType(QObject *parent = nullptr);

    virtual void paint(const PropertyBrowserItem *item, QPainter *painter, const QRect &rect) const override;

    virtual PropertyBrowserDialog *createDialog(const PropertyBrowserItem *item, QWidget *parent) const override;
};

class EnumPropertyBrowserType : public PropertyBrowserType
{
public:
    EnumPropertyBrowserType(const QStringList &values, QObject *parent = nullptr);
    EnumPropertyBrowserType(const QList<QPair<int, QString> > &values, QObject *parent = nullptr);

    virtual QString valueText(const PropertyBrowserItem *item) const override;

    virtual PropertyBrowserEditor *createEditor(const PropertyBrowserItem *item, QWidget *parent) const override;

private:
    pcx::aligned_store<64> cache;
};

}

#endif // QPXPROPERTYBROWSERTYPE_H
