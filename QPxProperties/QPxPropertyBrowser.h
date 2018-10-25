#ifndef QPX_PROPERTYBROWSER_H
#define QPX_PROPERTYBROWSER_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

namespace QPx
{

class PropertyBrowser : public QWidget
{
    Q_OBJECT

public:
    PropertyBrowser(QWidget *parent = nullptr);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    pcx::aligned_store<16> cache;
};

}

#endif // QPX_PROPERTYBROWSER_H
