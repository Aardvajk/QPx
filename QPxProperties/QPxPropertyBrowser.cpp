#include "QPxProperties/QPxPropertyBrowser.h"

#include "QPxCore/QPxTreeModel.h"

#include "internal/qpx_property_browser_model.h"

#include <QtWidgets/QTreeView>
#include <QtWidgets/QHeaderView>

namespace
{

class Cache
{
public:
    explicit Cache(QWidget *parent);

    qpx_property_browser_model *model;
    QTreeView *tree;
};

Cache::Cache(QWidget *parent) : model(new qpx_property_browser_model(parent)), tree(new QTreeView(parent))
{
    tree->header()->hide();
    tree->setModel(model);
}

}

QPx::PropertyBrowser::PropertyBrowser(QWidget *parent) : QWidget(parent)
{
    cache.alloc<Cache>(this);
}

void QPx::PropertyBrowser::resizeEvent(QResizeEvent *event)
{
    cache.get<Cache>().tree->setGeometry(rect());
}
