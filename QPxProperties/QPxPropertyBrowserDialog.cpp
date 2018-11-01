#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QColorDialog>

QPx::PropertyBrowserDialog::PropertyBrowserDialog(QWidget *parent) : QDialog(parent)
{
}

QPx::ColorPropertyBrowserDialog::ColorPropertyBrowserDialog(QWidget *parent)
{
    cache.alloc<QColor>();
    setAttribute(Qt::WA_DontShowOnScreen);
}

QVariant QPx::ColorPropertyBrowserDialog::value() const
{
    return QVariant::fromValue(cache.get<QColor>());
}

void QPx::ColorPropertyBrowserDialog::setValue(const QVariant &value)
{
    cache.get<QColor>() = qvariant_cast<QColor>(value);
}

int QPx::ColorPropertyBrowserDialog::exec()
{
    auto &color = cache.get<QColor>();

    QColorDialog d(this);
    d.setCurrentColor(color);

    int r = d.exec();
    if(r == QDialog::Accepted)
    {
        color = d.currentColor();
    }

    return r;
}
