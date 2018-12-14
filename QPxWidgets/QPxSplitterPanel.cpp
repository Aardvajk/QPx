#include "QPxWidgets/QPxSplitterPanel.h"

#include "QPxWidgets/QPxPalette.h"
#include "QPxWidgets/QPxLineSplitter.h"

#include <QtWidgets/QLayout>

QPx::SplitterPanel::SplitterPanel(QWidget *parent) : QWidget(parent)
{
}

void QPx::SplitterPanel::split(Qt::Orientation orientation, QWidget *widget)
{
    auto splitter = new QPx::LineSplitter(orientation);
    setPaletteColor(splitter, QPalette::Background, palette().color(QPalette::Background));

    auto parentSplitter = dynamic_cast<QSplitter*>(parentWidget());
    QList<int> parentSizes;

    if(parentSplitter)
    {
        parentSizes = parentSplitter->sizes();
        parentSplitter->insertWidget(parentSplitter->indexOf(this), splitter);
    }
    else if(parentWidget()->layout())
    {
        parentWidget()->layout()->replaceWidget(this, splitter);
    }

    splitter->addWidget(this);
    splitter->addWidget(widget);

    splitter->setSizes({ splitter->width() / 2, splitter->width() / 2 });

    if(parentSplitter)
    {
        parentSplitter->setSizes(parentSizes);
    }
}

void QPx::SplitterPanel::closeEvent(QCloseEvent *event)
{
    if(auto splitter = dynamic_cast<QSplitter*>(parentWidget()))
    {
        auto other = splitter->widget(splitter->indexOf(this) == 0 ? 1 : 0);

        if(auto parentSplitter = dynamic_cast<QSplitter*>(splitter->parentWidget()))
        {
            parentSplitter->insertWidget(parentSplitter->indexOf(splitter), other);
        }
        else if(splitter->parentWidget()->layout())
        {
            splitter->parentWidget()->layout()->replaceWidget(splitter, other);
        }

        delete splitter;
    }

    QWidget::closeEvent(event);
}
