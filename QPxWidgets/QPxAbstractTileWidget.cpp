#include "QPxAbstractTileWidget.h"

#include <QtCore/QEvent>

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include <QtWidgets/QScrollBar>

namespace
{

class Cache
{
public:
    Cache(QPx::AbstractTileWidget *widget, const QSize &dims, int count) : widget(widget), dims(dims), count(count), hover(-1), down(-1), inDown(false), index(-1) { }

    void init();
    void recalculate();
    void ensureVisible(int tile);

    int coordToIndex(const QPoint &pos) const;
    QPoint indexToTile(int index) const;
    QRect indexToRect(int index) const;

    QPx::AbstractTileWidget *widget;
    QSize dims;
    int count;

    int hover;
    int down;
    bool inDown;
    int index;
};

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

void Cache::ensureVisible(int tile)
{
    if(tile < 0 || tile >= count) return;

    QPoint c = indexToTile(tile);

    int top = c.y() * dims.height();
    int bottom = top + dims.height();

    int value = widget->verticalScrollBar()->value();

    if(value > top)
    {
        widget->verticalScrollBar()->setValue(top);
    }
    else if(bottom > value + widget->viewport()->height())
    {
        widget->verticalScrollBar()->setValue(bottom - widget->viewport()->height());
    }
}

int Cache::coordToIndex(const QPoint &pos) const
{
    int across = widget->viewport()->width() / dims.width();

    int cx = pos.x() / dims.width();
    int cy = (pos.y() + widget->verticalScrollBar()->value()) / dims.height();

    int index = (cy * across) + cx;

    if(cx >= across || index >= count) return -1;

    return index;
}

QPoint Cache::indexToTile(int index) const
{
    int across = widget->viewport()->width() / dims.width();
    int rows = count / across;

    if(count % across) ++rows;

    int cx = index % across;
    int cy = index / across;

    if(cx >= across || cy >= rows) return QPoint();

    return QPoint(cx, cy);
}

QRect Cache::indexToRect(int index) const
{
    auto p = indexToTile(index);
    if(!p.isNull())
    {
        return QRect(p.x() * dims.width(), (p.y() * dims.height()) - widget->verticalScrollBar()->value(), dims.width(), dims.height());
    }
    
    return QRect();
}

}

QPx::AbstractTileWidget::AbstractTileWidget(QWidget *parent) : QAbstractScrollArea(parent)
{
    cache.alloc<Cache>(this, QSize(32, 32), 0);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    viewport()->setMinimumSize(cache.get<Cache>().dims);

    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);

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

int QPx::AbstractTileWidget::selectedIndex() const
{
    return cache.get<Cache>().index;
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

void QPx::AbstractTileWidget::setSelectedIndex(int index)
{
    auto &c = cache.get<Cache>();

    if(index >= 0 && index < c.count && index != c.index)
    {
        c.index = index;
        c.ensureVisible(index);

        viewport()->update();

        emit selectedIndexChanged(index);
    }
}

bool QPx::AbstractTileWidget::event(QEvent *event)
{
    auto &c = cache.get<Cache>();

    if(event->type() == QEvent::Leave)
    {
        c.hover = -1;
        c.down = -1;
        c.inDown = false;
        
        viewport()->update();
    }
    else if(event->type() == QEvent::MouseMove)
    {
        auto m = static_cast<QMouseEvent*>(event);

        c.hover = c.coordToIndex(m->pos());
        c.inDown = c.indexToRect(c.down).contains(m->pos());

        viewport()->update();
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        auto m = static_cast<QMouseEvent*>(event);
        if(m->button() == Qt::LeftButton)
        {
            c.down = c.coordToIndex(static_cast<QMouseEvent*>(event)->pos());
            c.inDown = true;
            
            viewport()->update();
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *m = static_cast<QMouseEvent*>(event);
        if(m->button() == Qt::LeftButton)
        {
            if(c.inDown)
            {
                setSelectedIndex(c.down);
            }

            c.down = -1;
            c.inDown = false;
            
            viewport()->update();
        }
    }

    return QAbstractScrollArea::event(event);
}

void QPx::AbstractTileWidget::resizeEvent(QResizeEvent*)
{
    auto &c = cache.get<Cache>();

    c.recalculate();
    if(c.index != -1)
    {
        c.ensureVisible(c.index);
    }

    viewport()->update();
}

void QPx::AbstractTileWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(viewport());
    painter.fillRect(viewport()->rect(), palette().color(QPalette::Base));

    auto &c = cache.get<Cache>();

    int across = viewport()->width() / c.dims.width();
    int down = (viewport()->height() / c.dims.height()) + 2;

    int cy = verticalScrollBar()->value() / c.dims.height();
    int sy = verticalScrollBar()->value() % c.dims.height();

    int index = cy * across;

    for(int y = 0; y < down && index < c.count; ++y)
    {
        for(int x = 0; x < across && index < c.count; ++x)
        {
            QRect rect(x * c.dims.width(), (y * c.dims.height()) - sy, c.dims.width(), c.dims.height());

            if((index == c.hover && c.down == -1) || (index == c.down && !c.inDown))
            {
                painter.setPen(QColor(153, 209, 255));
                painter.setBrush(QColor(229, 243, 251));

                painter.drawRect(rect);
            }

            if(index == c.index || (index == c.down && c.inDown))
            {
                painter.setPen(QColor(112, 192, 231));
                painter.setBrush(QColor(204, 232, 255));
            
                painter.drawRect(rect);
            }

            paintTile(painter, index, rect);
            
            ++index;
        }
    }
}

void QPx::AbstractTileWidget::scrollChanged(int value)
{
    cache.get<Cache>().hover = cache.get<Cache>().coordToIndex(viewport()->mapFromGlobal(QCursor::pos()));
    update();
}
