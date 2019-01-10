#include "QPxValueButtons.h"

QPx::BoolButton::BoolButton(const QString &text, bool value, QWidget *parent) : QPushButton(text, parent)
{
    cache.alloc<bool>(value);
    connect(this, SIGNAL(clicked()), SLOT(buttonClicked()));
}

void QPx::BoolButton::buttonClicked()
{
    emit valueClicked(cache.get<bool>());
}
