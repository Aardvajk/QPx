#include "QPxWidgets/QPxLineSplitter.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include <QtWidgets/QSplitterHandle>

namespace
{

class LineSplitterHandle : public QSplitterHandle
{
public:
    LineSplitterHandle(Qt::Orientation orientation, QSplitter *parent);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};

LineSplitterHandle::LineSplitterHandle(Qt::Orientation orientation, QSplitter *parent) : QSplitterHandle(orientation, parent)
{
    setMask(QRegion(contentsRect()));
    setAttribute(Qt::WA_MouseNoMask, true);
}

void LineSplitterHandle::resizeEvent(QResizeEvent *event)
{
    orientation() == Qt::Horizontal ? setContentsMargins(2, 0, 2, 0) : setContentsMargins(0, 2, 0, 2);

    setMask(QRegion(contentsRect()));
    QSplitterHandle::resizeEvent(event);
}

void LineSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), parentWidget()->palette().color(QPalette::Background));
}

}

QPx::LineSplitter::LineSplitter(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{
    setHandleWidth(1);
    setChildrenCollapsible(false);

    auto p = palette();
    p.setColor(QPalette::Background, QColor(64, 64, 64));

    setPalette(p);
}

QSplitterHandle *QPx::LineSplitter::createHandle()
{
    return new LineSplitterHandle(orientation(), this);
}
