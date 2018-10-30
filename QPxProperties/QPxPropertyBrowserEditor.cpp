#include "QPxProperties/QPxPropertyBrowserEditor.h"

#include <QtGui/QIntValidator>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>

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
