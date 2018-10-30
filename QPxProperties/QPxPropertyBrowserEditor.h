#ifndef QPX_PROPERTYBROWSEREDITOR_H
#define QPX_PROPERTYBROWSEREDITOR_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

class QLineEdit;
class QComboBox;

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
    QLineEdit *edit;
};

class IntPropertyBrowserEditor : public StringPropertyBrowserEditor
{
    Q_OBJECT

public:
    explicit IntPropertyBrowserEditor(QWidget *parent = nullptr);

    virtual QVariant value() const override;
};

class FloatPropertyBrowserEditor : public StringPropertyBrowserEditor
{
    Q_OBJECT

public:
    explicit FloatPropertyBrowserEditor(QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;
};

class EnumPropertyBrowserEditor : public PropertyBrowserEditor
{
    Q_OBJECT

public:
    EnumPropertyBrowserEditor(const QMap<int, QString> &values, QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

private:
    QComboBox *combo;
};

}

#endif // QPX_PROPERTYBROWSEREDITOR_H
