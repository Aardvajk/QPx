#ifndef QPX_PROPERTYBROWSERDIALOG_H
#define QPX_PROPERTYBROWSERDIALOG_H

#include <QtCore/QVariant>

#include <QtWidgets/QColorDialog>

#include <pcx/aligned_store.h>

namespace QPx
{

class PropertyBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertyBrowserDialog(QWidget *parent = nullptr);

    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant &value) = 0;
};

class ProxyPropertyBrowserDialog : public PropertyBrowserDialog
{
    Q_OBJECT

public:
    explicit ProxyPropertyBrowserDialog(QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

protected:
    pcx::aligned_store<16> cache;
};

class ColorPropertyBrowserDialog : public ProxyPropertyBrowserDialog
{
    Q_OBJECT

public:
    using ProxyPropertyBrowserDialog::ProxyPropertyBrowserDialog;

public slots:
    virtual int exec() override;
};

}

#endif // QPX_PROPERTYBROWSERDIALOG_H
