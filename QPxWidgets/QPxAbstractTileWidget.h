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
    explicit AbstractTileWidget(QWidget *parent = nullptr);

    QSize dimensions() const;
    int count() const;
    int selectedIndex() const;

signals:
    void selectedIndexChanged(int value);

public slots:
    void setDimensions(const QSize &dimensions);
    void setCount(int count);
    void setSelectedIndex(int index);
    
protected:
    virtual void paintTile(QPainter &painter, int index, const QRect &rect) = 0;

    virtual bool event(QEvent *event) override;
    virtual void resizeEvent(QResizeEvent*) override;
    virtual void paintEvent(QPaintEvent*) override;

private slots:
    void scrollChanged(int value);

private:
    pcx::aligned_store<40> cache;
};

}

#endif // QPXABSTRACTTILEWIDGET_H
