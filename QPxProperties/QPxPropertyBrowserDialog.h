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

class ColorPropertyBrowserDialog : public PropertyBrowserDialog
{
    Q_OBJECT

public:
    explicit ColorPropertyBrowserDialog(QWidget *parent = nullptr);

    virtual QVariant value() const override;
    virtual void setValue(const QVariant &value) override;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_PROPERTYBROWSERDIALOG_H
