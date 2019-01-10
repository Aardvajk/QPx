#include "QPxValueButtons.h"

QPx::ValueButton::ValueButton(const QString &text, const QVariant &value, QWidget *parent) : QPushButton(text, parent)
{
    cache.alloc<QVariant>(value);
    connect(this, SIGNAL(clicked()), SLOT(buttonClicked()));
}

void QPx::ValueButton::buttonClicked()
{
    emit clicked(cache.get<QVariant>());
}
