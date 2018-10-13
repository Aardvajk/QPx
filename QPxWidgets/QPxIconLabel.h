#ifndef QPX_ICONLABEL_H
#define QPX_ICONLABEL_H

#include <QtGui/QIcon>

#include <QtWidgets/QLabel>

#include <pcx/aligned_store.h>

namespace QPx
{

class IconLabel : public QLabel
{
public:
    IconLabel(QWidget *parent = nullptr);

    QIcon icon() const;

public slots:
    void setIcon(const QIcon &icon);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    pcx::aligned_store<64> cache;
};

}

#endif // QPX_ICONLABEL_H
