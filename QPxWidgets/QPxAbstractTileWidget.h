#ifndef QPXABSTRACTTILEWIDGET_H
#define QPXABSTRACTTILEWIDGET_H

#include <QtWidgets/QAbstractScrollArea>

#include <pcx/aligned_store.h>

namespace QPx
{

class AbstractTileWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    AbstractTileWidget(QWidget *parent = nullptr);
    AbstractTileWidget(const QSize &dimensions, int count, QWidget *parent = nullptr);

    QSize dimensions() const;
    int count() const;

public slots:
    void setDimensions(const QSize &dimensions);
    void setCount(int count);
    
protected:
    virtual void paintTile(QPainter &painter, int index, const QRect &rect) = 0;

    virtual void resizeEvent(QResizeEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

private slots:
    void scrollChanged(int value);

private:
    pcx::aligned_store<32> cache;
};

}

#endif // QPXABSTRACTTILEWIDGET_H
