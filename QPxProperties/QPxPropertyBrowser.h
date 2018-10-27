#ifndef QPX_PROPERTYBROWSER_H
#define QPX_PROPERTYBROWSER_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

namespace QPx
{

class PropertyBrowserModel;

class PropertyBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyBrowser(QWidget *parent = nullptr);

    void setModel(PropertyBrowserModel *model);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    pcx::aligned_store<8> cache;
};

}

#endif // QPX_PROPERTYBROWSER_H
