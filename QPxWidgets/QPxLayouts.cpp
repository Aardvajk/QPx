#include "QPxWidgets/QPxLayouts.h"

QPx::VBoxLayout::VBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::VBoxLayout::VBoxLayout(int border, int spacing, QWidget *parent) : QVBoxLayout(parent)
{
    setMargin(border);
    setSpacing(spacing);
}

QPx::HBoxLayout::HBoxLayout(QWidget *parent) : QHBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::HBoxLayout::HBoxLayout(int border, int spacing, QWidget *parent) : QHBoxLayout(parent)
{
    setMargin(border);
    setSpacing(spacing);
}

QPx::LayoutWidget::LayoutWidget(Qt::Orientation orientation, QWidget *parent)
{
    switch(orientation)
    {
        case Qt::Horizontal: setLayout(new HBoxLayout());
        case Qt::Vertical: setLayout(new VBoxLayout());
    }
}

void QPx::LayoutWidget::addWidget(QWidget *widget, int stretch)
{
    static_cast<QBoxLayout*>(layout())->addWidget(widget, stretch);
}

QPx::Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{
    setHandleWidth(3);
}

void QPx::Splitter::addWidget(QWidget *widget, int stretch)
{
    QSplitter::addWidget(widget);

    if(stretch)
    {
        setStretchFactor(count() - 1, stretch);
    }
}

QPx::FramedWidget::FramedWidget(QWidget *widget, QWidget *parent) : QFrame(parent)
{
    setFrameShape(QFrame::StyledPanel);

    auto layout = new VBoxLayout(this);
    layout->addWidget(widget);
}
