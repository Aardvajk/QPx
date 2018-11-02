#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QColorDialog>

QPx::PropertyBrowserDialog::PropertyBrowserDialog(QWidget *parent) : QDialog(parent)
{
}

QPx::ProxyPropertyBrowserDialog::ProxyPropertyBrowserDialog(QWidget *parent)
{
    cache.alloc<QVariant>();
    setAttribute(Qt::WA_DontShowOnScreen);
}

QVariant QPx::ProxyPropertyBrowserDialog::value() const
{
    return cache.get<QVariant>();
}

void QPx::ProxyPropertyBrowserDialog::setValue(const QVariant &value)
{
    cache.get<QVariant>() = value;
}

int QPx::ColorPropertyBrowserDialog::exec()
{
    auto &value = cache.get<QVariant>();

    QColorDialog d(this);
    d.setCurrentColor(qvariant_cast<QColor>(value));

    int r = d.exec();
    if(r == QDialog::Accepted)
    {
        value = QVariant::fromValue(d.currentColor());
    }

    return r;
}
