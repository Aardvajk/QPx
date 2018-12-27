#include "QPxWidgets/QPxSplitterPanel.h"

#include "QPxWidgets/QPxLineSplitter.h"
#include "QPxWidgets/QPxLayouts.h"

#include <QtWidgets/QLayout>

QPx::SplitterPanel::SplitterPanel(QWidget *parent) : QWidget(parent)
{
}

void QPx::SplitterPanel::split(Qt::Orientation orientation, QWidget *widget)
{
    auto splitter = createSplitter(orientation);

    auto parentSplitter = qobject_cast<QSplitter*>(parentWidget());
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

QSplitter *QPx::SplitterPanel::createSplitter(Qt::Orientation orientation) const
{
    return new QPx::LineSplitter(orientation);
}

void QPx::SplitterPanel::closeEvent(QCloseEvent *event)
{
    if(auto splitter = qobject_cast<QSplitter*>(parentWidget()))
    {
        auto other = splitter->widget(splitter->indexOf(this) == 0 ? 1 : 0);

        if(auto parentSplitter = qobject_cast<QSplitter*>(splitter->parentWidget()))
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
