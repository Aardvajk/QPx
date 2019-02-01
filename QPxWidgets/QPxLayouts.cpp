#include "QPxWidgets/QPxLayouts.h"

namespace
{

void clearLayout(QLayout *layout)
{
    while(layout->count())
    {
        auto item = layout->takeAt(0);

        delete item->layout();
        delete item->widget();

        delete item;
    }
}

}

QPx::VBoxLayout::VBoxLayout(QWidget *parent) : QVBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::VBoxLayout::VBoxLayout(pcx::optional<int> border, pcx::optional<int> spacing, QWidget *parent) : QVBoxLayout(parent)
{
    if(border) setMargin(*border);
    if(spacing) setSpacing(*spacing);
}

void QPx::VBoxLayout::clear()
{
    clearLayout(this);
}

QPx::HBoxLayout::HBoxLayout(QWidget *parent) : QHBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::HBoxLayout::HBoxLayout(pcx::optional<int> border, pcx::optional<int> spacing, QWidget *parent) : QHBoxLayout(parent)
{
    if(border) setMargin(*border);
    if(spacing) setSpacing(*spacing);
}

void QPx::HBoxLayout::clear()
{
    clearLayout(this);
}

QPx::FormLayout::FormLayout(QWidget *parent) : QFormLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

QPx::FormLayout::FormLayout(pcx::optional<int> border, pcx::optional<int> spacing, QWidget *parent) : QFormLayout(parent)
{
    if(border) setMargin(*border);
    if(spacing) setSpacing(*spacing);
}

void QPx::FormLayout::addRightAlignedWidgetImp(QWidget *widget)
{
    auto layout = addTypedRow(new LayoutWidget(Qt::Horizontal));

    layout->addStretch();
    layout->addWidget(widget);
}

QPx::StackedLayout::StackedLayout(QWidget *parent) : QStackedLayout(parent)
{
}

QPx::LayoutWidget::LayoutWidget(Qt::Orientation orientation, QWidget *parent)
{
    switch(orientation)
    {
        case Qt::Horizontal: setLayout(new HBoxLayout()); break;
        case Qt::Vertical: setLayout(new VBoxLayout()); break;
    }
}

void QPx::LayoutWidget::addStretch()
{
    static_cast<QBoxLayout*>(layout())->addStretch();
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

QPx::DialogButtonBox::DialogButtonBox(QWidget *parent) : QDialogButtonBox(parent)
{

}

QPx::FramedWidget::FramedWidget(QWidget *widget, QWidget *parent) : QFrame(parent)
{
    setFrameShape(QFrame::StyledPanel);

    auto layout = new VBoxLayout(this);
    layout->addWidget(widget);
}
