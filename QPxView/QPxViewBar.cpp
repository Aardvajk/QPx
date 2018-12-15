#include "QPxView/QPxViewBar.h"

#include "QPxWidgets/QPxLayouts.h"

#include <QtGui/QPainter>

#include <QtWidgets/QApplication>

namespace
{

class Separator : public QWidget
{
public:
    Separator(Qt::Orientation orientation, const QSize &defaultButtonSize);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Qt::Orientation orientation;
};

Separator::Separator(Qt::Orientation orientation, const QSize &defaultButtonSize) : orientation(orientation)
{
    if(orientation == Qt::Horizontal)
    {
        setFixedSize(4, defaultButtonSize.height());
    }
    else
    {
        setFixedSize(defaultButtonSize.width(), 4);
    }
}

void Separator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(qvariant_cast<QColor>(QApplication::instance()->property("qpx-view-ui-border")));

    if(orientation == Qt::Horizontal)
    {
        painter.drawLine(1, 4, 1, height() - 5);
    }
    else
    {
        painter.drawLine(4, 1, width() - 5, 1);
    }
}

class Cache
{
public:
    Cache(Qt::Orientation orientation, QPx::ViewBar::Type type) : orientation(orientation), type(type), defaultButtonSize(type == QPx::ViewBar::Type::Small ? QSize(22, 22) : QSize(70, 52)) { }

    Qt::Orientation orientation;
    QPx::ViewBar::Type type;
    QSize defaultButtonSize;
};

}

QPx::ViewBar::ViewBar(Qt::Orientation orientation, Type type, QWidget *parent) : QWidget(parent)
{
    auto &c = cache.alloc<Cache>(orientation, type);

    if(orientation == Qt::Horizontal)
    {
        new HBoxLayout(this);
        setFixedHeight(c.defaultButtonSize.height());
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    else
    {
        new VBoxLayout(this);
        setFixedWidth(c.defaultButtonSize.width());
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    }
}

void QPx::ViewBar::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

void QPx::ViewBar::addSeparator()
{
    auto &c = cache.get<Cache>();
    addWidget(new Separator(c.orientation, c.defaultButtonSize));
}

void QPx::ViewBar::addStretch()
{
    auto w = new QWidget();
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    addWidget(w);
}

Qt::Orientation QPx::ViewBar::orientation() const
{
    return cache.get<Cache>().orientation;
}

QPx::ViewBar::Type QPx::ViewBar::type() const
{
    return cache.get<Cache>().type;
}

QSize QPx::ViewBar::defaultButtonSize() const
{
    return cache.get<Cache>().defaultButtonSize;
}

void QPx::ViewBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), qvariant_cast<QColor>(QApplication::instance()->property("qpx-view-ui-panel")));
}
