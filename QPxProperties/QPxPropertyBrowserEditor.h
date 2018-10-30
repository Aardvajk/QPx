#ifndef QPX_PROPERTYBROWSEREDITOR_H
#define QPX_PROPERTYBROWSEREDITOR_H

#include <QtWidgets/QWidget>

class QLineEdit;

namespace QPx
{

class PropertyBrowserEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyBrowserEditor(QWidget *parent = nullptr);

    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant &value) = 0;
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

}

#endif // QPX_PROPERTYBROWSEREDITOR_H
