#include "QPxWidgets/QPxLayouts.h"

QPx::VBoxLayout::VBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
    setMargin(0);
}

QPx::HBoxLayout::HBoxLayout(QWidget *parent) : QHBoxLayout(parent)
{
    setMargin(0);
}

