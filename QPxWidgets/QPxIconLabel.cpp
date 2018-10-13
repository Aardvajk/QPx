#include "QPxWidgets/QPxIconLabel.h"

#include <QtGui/QPainter>

#include <QtWidgets/QStyle>

namespace
{

class Cache
{
public:
    Cache(const QMargins &margins) : margins(margins) { }

    QMargins margins;
    QIcon icon;
};

}

QPx::IconLabel::IconLabel(QWidget *parent) : QLabel(parent)
{
    cache.alloc<Cache>(contentsMargins());
}

QIcon QPx::IconLabel::icon() const
{
    return cache.get<Cache>().icon;
}

void QPx::IconLabel::setIcon(const QIcon &icon)
{
    cache.get<Cache>().icon = icon;
    update();
}

void QPx::IconLabel::resizeEvent(QResizeEvent *event)
{
    auto m = cache.get<Cache>().margins;
    m.setLeft(rect().height() + style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing));

    setContentsMargins(m);

    QLabel::resizeEvent(event);
}

void QPx::IconLabel::paintEvent(QPaintEvent *event)
{
    auto &icon = cache.get<Cache>().icon;
    if(!icon.isNull())
    {
        int h = rect().height();
        auto pix = icon.pixmap(h);

        QPainter painter(this);
        painter.drawPixmap(0, 0, pix.scaledToHeight(h, Qt::SmoothTransformation));
    }

    QLabel::paintEvent(event);
}


