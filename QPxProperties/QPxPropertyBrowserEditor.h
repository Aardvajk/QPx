#ifndef QPX_PROPERTYBROWSEREDITOR_H
#define QPX_PROPERTYBROWSEREDITOR_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

namespace QPx
{

class PropertyBrowserEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyBrowserEditor(QWidget *parent = nullptr);

    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant &value) = 0;

signals:
    void commit();
};

class StringPropertyBrowserEditor : public PropertyBrowserEditor
{
    Q_OBJECT

public:
    explicit StringPropertyBrowserEditor(QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

protected:
    pcx::aligned_store<8> cache;
};

class IntPropertyBrowserEditor : public StringPropertyBrowserEditor
{
    Q_OBJECT

public:
    explicit IntPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent = nullptr);

    virtual QVariant value() const override;
};

class FloatPropertyBrowserEditor : public StringPropertyBrowserEditor
{
    Q_OBJECT

public:
    explicit FloatPropertyBrowserEditor(const QVariant &min, const QVariant &max, QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;
};

class AbstractEnumPropertyBrowserType;

class EnumPropertyBrowserEditor : public PropertyBrowserEditor
{
    Q_OBJECT

public:
    EnumPropertyBrowserEditor(const AbstractEnumPropertyBrowserType *type, const QMap<int, QString> &values, QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

private:
    pcx::aligned_store<16> cache;
};

}

#endif // QPX_PROPERTYBROWSEREDITOR_H
