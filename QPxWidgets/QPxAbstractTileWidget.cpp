#include "QPxAbstractTileWidget.h"

#include <QtGui/QPainter>

#include <QtWidgets/QScrollBar>

namespace
{

class Cache
{
public:
    Cache(QPx::AbstractTileWidget *widget, const QSize &dims, int count);

    void init();
    void recalculate();

    QPx::AbstractTileWidget *widget;
    QSize dims;
    int count;
};

Cache::Cache(QPx::AbstractTileWidget *widget, const QSize &dims, int count) : widget(widget), dims(dims), count(count)
{
    widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    widget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    widget->viewport()->setMinimumSize(dims);

    widget->setMouseTracking(true);
}

void Cache::recalculate()
{
    int across = widget->viewport()->width() / dims.width();
    int rows = count / across;

    if(count % across) ++rows;

    int total = dims.height() * rows;

    widget->verticalScrollBar()->setRange(0, total - widget->viewport()->height());
    widget->verticalScrollBar()->setSingleStep(dims.height());
    widget->verticalScrollBar()->setPageStep(widget->viewport()->height());
}

}

QPx::AbstractTileWidget::AbstractTileWidget(QWidget *parent) : QAbstractScrollArea(parent)
{
    cache.alloc<Cache>(this, QSize(32, 32), 0);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(scrollChanged(int)));
}

QPx::AbstractTileWidget::AbstractTileWidget(const QSize &dimensions, int count, QWidget *parent) : QAbstractScrollArea(parent)
{
    cache.alloc<Cache>(this, dimensions, count);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(scrollChanged(int)));
}

QSize QPx::AbstractTileWidget::dimensions() const
{
    return cache.get<Cache>().dims;
}

int QPx::AbstractTileWidget::count() const
{
    return cache.get<Cache>().count;
}

void QPx::AbstractTileWidget::setDimensions(const QSize &dimensions)
{
    if(cache.get<Cache>().dims != dimensions)
    {
        cache.get<Cache>().dims = dimensions;
        cache.get<Cache>().recalculate();
        
        viewport()->update();
    }
}

void QPx::AbstractTileWidget::setCount(int count)
{
    if(cache.get<Cache>().count != count)
    {
        cache.get<Cache>().count = count;
        cache.get<Cache>().recalculate();

        viewport()->update();
    }
}

void QPx::AbstractTileWidget::resizeEvent(QResizeEvent*)
{
    cache.get<Cache>().recalculate();
    viewport()->update();
}

void QPx::AbstractTileWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(viewport());
    painter.fillRect(viewport()->rect(), palette().color(QPalette::Base));

    const auto &dims = cache.get<Cache>().dims;

    int across = viewport()->width() / dims.width();
    int down = (viewport()->height() / dims.height()) + 2;

    int cy = verticalScrollBar()->value() / dims.height();
    int sy = verticalScrollBar()->value() % dims.height();

    int index = cy * across;

    for(int y = 0; y < down && index < cache.get<Cache>().count; ++y)
    {
        for(int x = 0; x < across && index < cache.get<Cache>().count; ++x)
        {
            paintTile(painter, index++, QRect(x * dims.width(), (y * dims.height()) - sy, dims.width(), dims.height()));
        }
    }
}

void QPx::AbstractTileWidget::scrollChanged(int value)
{
    update();
}
