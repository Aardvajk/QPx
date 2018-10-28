#include "QPxProperties/QPxPropertyBrowser.h"

#include "QPxProperties/QPxPropertyBrowserItem.h"
#include "QPxProperties/QPxPropertyBrowserModel.h"
#include "QPxProperties/QPxPropertyBrowserDelegate.h"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QHeaderView>

namespace
{

class Cache
{
public:
    explicit Cache(QWidget *parent);

    QTreeView *tree;
};

Cache::Cache(QWidget *parent) : tree(new QTreeView(parent))
{
    tree->header()->hide();
    tree->setItemDelegate(new QPx::PropertyBrowserDelegate(tree));
}

}

QPx::PropertyBrowser::PropertyBrowser(QWidget *parent)
{
    cache.alloc<Cache>(this);
}

void QPx::PropertyBrowser::setModel(PropertyBrowserModel *model)
{
    cache.get<Cache>().tree->setModel(model);
}

void QPx::PropertyBrowser::resizeEvent(QResizeEvent *event)
{
    cache.get<Cache>().tree->setGeometry(rect());
}
