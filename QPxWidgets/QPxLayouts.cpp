#include "QPxWidgets/QPxLayouts.h"

QPx::VBoxLayout::VBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::HBoxLayout::HBoxLayout(QWidget *parent) : QHBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::LayoutWidget::LayoutWidget(Qt::Orientation orientation, QWidget *parent)
{
    switch(orientation)
    {
        case Qt::Horizontal: setLayout(new HBoxLayout());
        case Qt::Vertical: setLayout(new VBoxLayout());
    }
}

void QPx::LayoutWidget::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}
