#include "QPxProperties/QPxPropertyEditor.h"

#include <QtGui/QIntValidator>

#include <QtWidgets/QLineEdit>

QPx::PropertyEditor::PropertyEditor(QWidget *parent) : QWidget(parent)
{
}

QPx::StringPropertyEditor::StringPropertyEditor(QWidget *parent) : PropertyEditor(parent), edit(new QLineEdit(this))
{
    setFocusProxy(edit);
}

QVariant QPx::StringPropertyEditor::value() const
{
    return edit->text();
}

void QPx::StringPropertyEditor::setValue(const QVariant &value)
{
    edit->setText(value.toString());
}

void QPx::StringPropertyEditor::resizeEvent(QResizeEvent *event)
{
    edit->setGeometry(rect());
}

QPx::IntPropertyEditor::IntPropertyEditor(QWidget *parent) : StringPropertyEditor(parent)
{
    edit->setValidator(new QIntValidator(this));
}

QVariant QPx::IntPropertyEditor::value() const
{
    return edit->text().toInt();
}
