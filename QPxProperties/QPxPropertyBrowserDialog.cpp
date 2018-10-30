#include "QPxProperties/QPxPropertyBrowserDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QColorDialog>

namespace
{

class ColorCache
{
public:
    explicit ColorCache(QWidget *parent) : dialog(new QColorDialog(parent)) { }

    QColorDialog *dialog;
};

}

QPx::PropertyBrowserDialog::PropertyBrowserDialog(QWidget *parent) : QDialog(parent)
{
}

QPx::ColorPropertyBrowserDialog::ColorPropertyBrowserDialog(QWidget *parent)
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    auto &c = cache.alloc<ColorCache>(this);
    c.dialog->setWindowFlags(Qt::Widget);
    c.dialog->setOptions(QColorDialog::DontUseNativeDialog);

    layout->addWidget(c.dialog);

    connect(c.dialog, SIGNAL(accepted()), SLOT(accept()));
    connect(c.dialog, SIGNAL(rejected()), SLOT(reject()));
}

QVariant QPx::ColorPropertyBrowserDialog::value() const
{
    return QVariant::fromValue(cache.get<ColorCache>().dialog->currentColor());
}

void QPx::ColorPropertyBrowserDialog::setValue(const QVariant &value)
{
    cache.get<ColorCache>().dialog->setCurrentColor(qvariant_cast<QColor>(value));
}
