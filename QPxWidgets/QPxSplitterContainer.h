#ifndef QPX_SPLITTERCONTAINER_H
#define QPX_SPLITTERCONTAINER_H

#include <QtWidgets/QWidget>

#include <pcx/aligned_store.h>

namespace QPx
{

class SplitterPanel;

class SplitterContainer : public QWidget
{
    Q_OBJECT

public:
    SplitterContainer(SplitterPanel *panel, QWidget *parent = nullptr);

private:
    pcx::aligned_store<16> cache;
};

}

#endif // QPX_SPLITTERCONTAINER_H
