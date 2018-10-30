#include "QPxProperties/QPxPropertyBrowserEditor.h"

#include "internal/qpx_combo_box.h"

#include <QtCore/QTimer>

#include <QtGui/QIntValidator>
#include <QtGui/QDoubleValidator>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>

QPx::PropertyBrowserEditor::PropertyBrowserEditor(QWidget *parent) : QWidget(parent)
{
}

QPx::StringPropertyBrowserEditor::StringPropertyBrowserEditor(QWidget *parent) : PropertyBrowserEditor(parent), edit(new QLineEdit(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(edit);
    setFocusProxy(edit);
}

QVariant QPx::StringPropertyBrowserEditor::value() const
{
    return edit->text();
}

void QPx::StringPropertyBrowserEditor::setValue(const QVariant &value)
{
    edit->setText(value.toString());
}

QPx::IntPropertyBrowserEditor::IntPropertyBrowserEditor(QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    edit->setValidator(new QIntValidator(this));
}

QVariant QPx::IntPropertyBrowserEditor::value() const
{
    return edit->text().toInt();
}

QPx::FloatPropertyBrowserEditor::FloatPropertyBrowserEditor(QWidget *parent) : StringPropertyBrowserEditor(parent)
{
    edit->setValidator(new QDoubleValidator(this));
}

QVariant QPx::FloatPropertyBrowserEditor::value() const
{
    return edit->text().toFloat();
}

void QPx::FloatPropertyBrowserEditor::setValue(const QVariant &value)
{
    edit->setText(QString::number(value.toFloat()));
}

QPx::EnumPropertyBrowserEditor::EnumPropertyBrowserEditor(const QMap<int, QString> &values, QWidget *parent) : PropertyBrowserEditor(parent), combo(new qpx_combo_box(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setMargin(0);

    foreach(int i, values.keys())
    {
        combo->addItem(values[i], i);
    }

    layout->addWidget(combo);
    setFocusProxy(combo);

    connect(combo, SIGNAL(currentIndexChanged(int)), SIGNAL(commit()));

    QTimer::singleShot(0, static_cast<qpx_combo_box*>(combo), SLOT(open()));
}

QVariant QPx::EnumPropertyBrowserEditor::value() const
{
    return combo->itemData(combo->currentIndex());
}

void QPx::EnumPropertyBrowserEditor::setValue(const QVariant &value)
{
    for(int i = 0; i < combo->count(); ++i)
    {
        if(combo->itemData(i) == value.toInt())
        {
            combo->setCurrentIndex(i);
            return;
        }
    }

    combo->setCurrentIndex(-1);
}
