#include "QPxWidgets/QPxSplitterPanel.h"

#include "QPxWidgets/QPxLineSplitter.h"

#include <QtCore/QTimer>

#include <QtGui/QCloseEvent>

#include <QtWidgets/QLayout>

namespace
{

void splitPanel(QPx::SplitterPanel *panel, Qt::Orientation orientation, QSplitter*(QPx::SplitterPanel::*cs)(Qt::Orientation), QPx::SplitterPanel*(QPx::SplitterPanel::*cf)() const)
{
    auto splitter = (panel->*cs)(orientation);

    auto parentSplitter = dynamic_cast<QSplitter*>(panel->parentWidget());
    QList<int> parentSizes;

    if(parentSplitter)
    {
        parentSizes = parentSplitter->sizes();
        parentSplitter->insertWidget(parentSplitter->indexOf(panel), splitter);
    }
    else if(panel->parentWidget()->layout())
    {
        panel->parentWidget()->layout()->replaceWidget(panel, splitter);
    }

    splitter->addWidget(panel);
    splitter->addWidget((panel->*cf)());

    splitter->setSizes({ splitter->width() / 2, splitter->width() / 2 });

    if(parentSplitter)
    {
        parentSplitter->setSizes(parentSizes);
    }
}

void setPaletteBackground(QWidget *widget, const QColor &color)
{
    auto p = widget->palette();
    p.setColor(QPalette::Background, color);

    widget->setPalette(p);
}

}

QPx::SplitterPanel::SplitterPanel(QWidget *parent) : QWidget(parent), closeFlag(false)
{
    setPaletteBackground(this, QColor(64, 64, 64));
}

void QPx::SplitterPanel::splitVertical()
{
    splitPanel(this, Qt::Vertical, &createSplitter, &clone);
}

void QPx::SplitterPanel::splitHorizontal()
{
    splitPanel(this, Qt::Horizontal, &createSplitter, &clone);
}

void QPx::SplitterPanel::closeEvent(QCloseEvent *event)
{
    if(!closeFlag)
    {
        QTimer::singleShot(0, this, SLOT(close()));
        closeFlag = true;

        event->ignore();
    }
    else
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
}

QSplitter *QPx::SplitterPanel::createSplitter(Qt::Orientation orientation)
{
    auto s = new QPx::LineSplitter(orientation);
    setPaletteBackground(s, palette().color(QPalette::Background));

    return s;
}
