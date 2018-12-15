#ifndef QPX_VIEWBARBUTTON_H
#define QPX_VIEWBARBUTTON_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

class QMenu;

namespace QPx
{

class ViewBar;

class ViewBarButton : public QWidget
{
    Q_OBJECT

public:
    ViewBarButton(const QPixmap &pixmap, ViewBar *parent);
    ViewBarButton(const QString &text, const QPixmap &pixmap, ViewBar *parent);

    void setMenu(QMenu *menu);
    void setCheckable(bool state);

    bool isCheckable() const;
    bool isChecked() const;

signals:
    void clicked();
    void toggled(bool state);

public slots:
    void click();
    void setChecked(bool state);
    void toggle();

protected:
    virtual bool event(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    void reset();

private:
    void buttonPressed();
    void buttonReleased();

    pcx::aligned_store<80> cache;
};

}

#endif // QPX_VIEWBARBUTTON_H
